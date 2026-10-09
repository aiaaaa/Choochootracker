// Receiver-only Ableton Link client: owns the status state machine and
// the serial-bumped getters the UI polls, delegating actual Link library
// work to the registered LinkBackend (see link_backend.h). Mirrors
// midi/midi_router.cpp's shape: global backend registration, per-state
// bookkeeping, atomics for cross-thread reads.

#include "link_client.h"
#include "link_backend.h"
#include "link_backend_internal.h"

#include <atomic>
#include <chrono>

// Discovery threshold: how long Connect waits before reporting noPeers.
// mDNS discovery typically completes within a second on a healthy LAN;
// 3 seconds covers slow Wi-Fi joins without making the UI feel stuck.
static constexpr uint64_t kDiscoveryThresholdMicros = 3000000;

// Registered backend (see linkClientSetBackend in link_backend.h).
static const LinkBackend* g_linkBackend = nullptr;

const LinkBackend* linkClientCurrentBackend(void) {
  return g_linkBackend;
}

void linkClientSetBackend(const LinkBackend* backend) {
  g_linkBackend = backend;
}

// Monotonic microsecond clock on the same scale as midiIoNowMicros()
// (std::chrono::steady_clock), so timestamps from both clocks compare
// directly.
uint64_t linkClientNowMicros(void) {
  return (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(
    std::chrono::steady_clock::now().time_since_epoch()).count();
}

// Default output latency until the platform audio setup overrides it
// (T39). Weak so corelib_audio.cpp can provide the measured device
// latency (spec.samples * 1e6 / spec.freq) with a strong definition;
// tests and stub platforms keep this 0 default.
__attribute__((weak)) uint64_t outputLatencyMicros(void) { return 0; }

namespace {

// Default backend when none registered: the real backend where Link is
// compiled in, the stub elsewhere (both expose linkBackendRealGet()).
const LinkBackend* activeBackend() {
  const LinkBackend* backend = linkClientCurrentBackend();
  return backend ? backend : linkBackendRealGet();
}

}  // namespace

struct LinkClientState {
  // --- UI-thread-owned lifecycle ---
  // enabled mirrors the backend's enable state so Disconnect/Deselect
  // can skip redundant backend calls.
  int enabled;

  // --- Status state machine (written on UI thread, read anywhere) ---
  std::atomic<LinkStatus> status;
  std::atomic<uint32_t> statusSerial;

  // --- Session snapshot (written by audio thread via capture, read by UI) ---
  std::atomic<float> sessionTempo;
  std::atomic<uint32_t> tempoSerial;
  std::atomic<int> peerCount;

  // --- Connect timing (UI thread) ---
  uint64_t connectStartedMicros;
  int connectPending;  // 1 while waiting for discovery to resolve

  // Backend-owned state (real backend: ableton::Link + HostTimeFilter).
  void* backendState;

  LinkClientState()
    : enabled(0),
      status(LinkStatus::disabled),
      statusSerial(0),
      sessionTempo(0.0f),
      tempoSerial(0),
      peerCount(0),
      connectStartedMicros(0),
      connectPending(0),
      backendState(nullptr) {}
};

static void setStatus(LinkClientState* state, LinkStatus status) {
  state->status.store(status, std::memory_order_release);
  state->statusSerial.fetch_add(1, std::memory_order_acq_rel);
}

LinkClientState* linkClientCreate(void) {
  LinkClientState* state = new LinkClientState();
  state->backendState = activeBackend()->create(activeBackend()->userdata);
  return state;
}

void linkClientDestroy(LinkClientState* state) {
  if (!state) return;
  if (state->enabled) {
    activeBackend()->disable(activeBackend()->userdata, state->backendState);
    state->enabled = 0;
  }
  activeBackend()->destroy(activeBackend()->userdata, state->backendState);
  delete state;
}

void linkClientSelect(LinkClientState* state) {
  setStatus(state, LinkStatus::selected);
}

void linkClientDeselect(LinkClientState* state) {
  if (state->enabled) {
    activeBackend()->disable(activeBackend()->userdata, state->backendState);
    state->enabled = 0;
  }
  state->connectPending = 0;
  state->sessionTempo.store(0.0f, std::memory_order_release);
  state->tempoSerial.fetch_add(1, std::memory_order_acq_rel);
  state->peerCount.store(0, std::memory_order_release);
  setStatus(state, LinkStatus::disabled);
}

void linkClientConnect(LinkClientState* state) {
  if (state->enabled) {
    // Already connected/connecting: nothing to do.
    return;
  }
  setStatus(state, LinkStatus::connecting);
  if (!activeBackend()->enable(activeBackend()->userdata, state->backendState)) {
    setStatus(state, LinkStatus::error);
    return;
  }
  state->enabled = 1;
  state->connectPending = 1;
  state->connectStartedMicros = linkClientNowMicros();
}

void linkClientDisconnect(LinkClientState* state) {
  if (state->enabled) {
    activeBackend()->disable(activeBackend()->userdata, state->backendState);
    state->enabled = 0;
  }
  state->connectPending = 0;
  state->sessionTempo.store(0.0f, std::memory_order_release);
  state->tempoSerial.fetch_add(1, std::memory_order_acq_rel);
  state->peerCount.store(0, std::memory_order_release);
  setStatus(state, LinkStatus::selected);
}

LinkStatus linkClientGetStatus(const LinkClientState* state, uint32_t* serial) {
  if (serial) *serial = state->statusSerial.load(std::memory_order_acquire);
  return state->status.load(std::memory_order_acquire);
}

int linkClientGetPeerCount(const LinkClientState* state) {
  return state->peerCount.load(std::memory_order_acquire);
}

float linkClientGetSessionTempo(const LinkClientState* state, uint32_t* serial) {
  if (serial) *serial = state->tempoSerial.load(std::memory_order_acquire);
  return state->sessionTempo.load(std::memory_order_acquire);
}

int linkClientCaptureAudioSessionState(LinkClientState* state, double* tempo, uint64_t* peers) {
  double capturedTempo = 0.0;
  uint64_t capturedPeers = 0;
  if (!activeBackend()->captureAudioSessionState(activeBackend()->userdata, state->backendState,
                                                 &capturedTempo, &capturedPeers)) {
    // Zero the out-params so callers never see stale values on failure.
    if (tempo) *tempo = 0.0;
    if (peers) *peers = 0;
    return 0;
  }
  // Publish the snapshot for the UI poll. Bump the tempo serial only
  // when the value actually changed so callers don't re-apply identical
  // tempos.
  const float previous = state->sessionTempo.load(std::memory_order_relaxed);
  const float next = (float)capturedTempo;
  state->sessionTempo.store(next, std::memory_order_release);
  if (next != previous) {
    state->tempoSerial.fetch_add(1, std::memory_order_acq_rel);
  }
  state->peerCount.store((int)capturedPeers, std::memory_order_release);
  if (tempo) *tempo = capturedTempo;
  if (peers) *peers = capturedPeers;
  return 1;
}

uint64_t linkClientRequestBeatAtLaunch(LinkClientState* state, int64_t quantum) {
  return activeBackend()->requestBeatAtLaunch(activeBackend()->userdata, state->backendState,
                                              (double)quantum);
}

// --- UI-thread poll resolution (called once per UI frame from app.cpp) ---

void linkClientPoll(LinkClientState* state) {
  if (!state->connectPending) return;

  const int peers = activeBackend()->numPeers(activeBackend()->userdata, state->backendState);
  if (peers > 0) {
    state->connectPending = 0;
    state->peerCount.store(peers, std::memory_order_release);
    setStatus(state, LinkStatus::connected);
    return;
  }
  const uint64_t now = linkClientNowMicros();
  if (now - state->connectStartedMicros >= kDiscoveryThresholdMicros) {
    state->connectPending = 0;
    setStatus(state, LinkStatus::noPeers);
  }
  // Otherwise stay in connecting until peers appear or the threshold hits.
}

void linkClientNotifyAudioFrame(LinkClientState* state, uint64_t sampleCounter, uint64_t hostTimeMicros) {
  activeBackend()->onAudioFrame(activeBackend()->userdata, state->backendState, sampleCounter,
                                hostTimeMicros);
}

void linkClientResetHostTimeFilter(LinkClientState* state) {
  activeBackend()->resetHostTimeFilter(activeBackend()->userdata, state->backendState);
}

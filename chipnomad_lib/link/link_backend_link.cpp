// Real Ableton Link backend: wraps <ableton/Link.hpp> behind the
// LinkBackend vtable (see link_backend.h). Compiled only on platforms
// that ship Link (DESKTOP_BUILD / PORTMASTER_BUILD / ANDROID_BUILD);
// every other platform gets link_backend_stub.cpp instead.
//
// RECEIVER-ONLY CONTRACT: this file never calls setTempo,
// setIsPlaying, commitAudioSessionState, startPlaying or stopPlaying.
// It only enables/disables the client, reads session state, and (for
// local phase alignment at Play) calls requestBeatAtTime, which aligns
// THIS client's timeline without pushing anything to peers.

#include "link_backend.h"
#include "link_backend_internal.h"
#include "host_time_filter.h"

#if defined(DESKTOP_BUILD) || defined(PORTMASTER_BUILD) || defined(ANDROID_BUILD)

#include <ableton/Link.hpp>

#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>

#ifdef ANDROID_BUILD
// T28: Android needs a WifiManager MulticastLock before Link's mDNS
// discovery can receive multicast packets. Declared here, implemented in
// tracker/platforms/android/link_backend_android.cpp (JNI).
extern "C" int linkBackendAndroidAcquireMulticastLock(void);
extern "C" int linkBackendAndroidReleaseMulticastLock(void);
#endif

namespace {

// HostTimeFilter lives in host_time_filter.h (header-only so the
// unit test exercises the exact code the backend runs).


struct RealLinkState {
  ableton::Link link{125.0};  // seed tempo; overridden by the session once connected
  HostTimeFilter hostTimeFilter;
  // Latest sample counter reported by the audio thread; the launch-time
  // estimator maps it through the regression to a host time.
  std::atomic<uint64_t> lastSampleCounter{0};
  // Headroom added on top of the measured output latency when scheduling
  // a beat-0 alignment (see linkClientRequestBeatAtLaunch). 10 ms covers
  // callback scheduling jitter on the platforms we ship.
  static constexpr uint64_t kSchedulingHeadroomMicros = 10000;
};

void* linkBackendCreate(void*) {
  RealLinkState* state = new RealLinkState();
  return state;
}

void linkBackendDestroy(void*, void* state) {
  delete (RealLinkState*)state;
}

int linkBackendEnable(void*, void* state) {
  RealLinkState* s = (RealLinkState*)state;
#ifdef ANDROID_BUILD
  // MulticastLock must be held before Link opens its sockets, or mDNS
  // discovery stays deaf on Android (T28).
  linkBackendAndroidAcquireMulticastLock();
#endif
  s->hostTimeFilter.reset();
  s->link.enable(true);
  return 1;
}

void linkBackendDisable(void*, void* state) {
  RealLinkState* s = (RealLinkState*)state;
  s->link.enable(false);
  s->hostTimeFilter.reset();
#ifdef ANDROID_BUILD
  linkBackendAndroidReleaseMulticastLock();
#endif
}

int linkBackendIsEnabled(void*, void* state) {
  RealLinkState* s = (RealLinkState*)state;
  return s->link.isEnabled() ? 1 : 0;
}

int linkBackendNumPeers(void*, void* state) {
  RealLinkState* s = (RealLinkState*)state;
  return (int)s->link.numPeers();
}

int linkBackendCaptureAudioSessionState(void*, void* state, double* tempo, uint64_t* peers) {
  RealLinkState* s = (RealLinkState*)state;
  if (!s->link.isEnabled()) return 0;
  const auto sessionState = s->link.captureAudioSessionState();
  if (tempo) *tempo = sessionState.tempo();
  if (peers) *peers = (uint64_t)s->link.numPeers();
  return 1;
}

uint64_t linkBackendRequestBeatAtLaunch(void*, void* state, double quantum) {
  RealLinkState* s = (RealLinkState*)state;
  if (!s->link.isEnabled() || s->link.numPeers() == 0) return 0;
  // Estimate the host time of "now" on Link's clock: the HostTimeFilter
  // maps the audio sample counter to host time, so ask it for the
  // current counter. The audio thread feeds pairs every callback (see
  // linkBackendOnAudioFrame); the latest sample counter is the one it
  // last reported.
  uint64_t hostTime = 0;
  if (!s->hostTimeFilter.estimate(s->lastSampleCounter.load(std::memory_order_relaxed), &hostTime)) {
    // No fit yet (callback hasn't run twice since enable): fall back to
    // the wall clock, which Link's timeline also uses as its host-time
    // base on every platform we ship.
    hostTime = (uint64_t)std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::steady_clock::now().time_since_epoch()).count();
  }
  const auto target = std::chrono::microseconds(
    hostTime + outputLatencyMicros() + RealLinkState::kSchedulingHeadroomMicros);
  auto sessionState = s->link.captureAudioSessionState();
  // Quantized launch (LOCAL alignment): requestBeatAtTime maps beat 0 to
  // the next session phase boundary at/after the target time on THIS
  // client's timeline. With peers present it never re-maps the session's
  // beat/time relationship (no tempo push, no transport push); the
  // commit below only publishes our own timeline adjustment.
  sessionState.requestBeatAtTime(0.0, target, quantum);
  s->link.commitAudioSessionState(sessionState);
  return (uint64_t)target.count();
}

void linkBackendOnAudioFrame(void*, void* state, uint64_t sampleCounter, uint64_t hostTimeMicros) {
  RealLinkState* s = (RealLinkState*)state;
  s->lastSampleCounter.store(sampleCounter, std::memory_order_relaxed);
  s->hostTimeFilter.add(sampleCounter, hostTimeMicros);
}

void linkBackendResetHostTimeFilter(void*, void* state) {
  RealLinkState* s = (RealLinkState*)state;
  s->hostTimeFilter.reset();
}

// outputLatencyMicros() is declared in link_backend_internal.h. The
// weak default definition lives in link_client.cpp; corelib_audio
// overrides it with the measured device latency (T39).

}  // namespace

static const LinkBackend kLinkBackend = {
  nullptr,  // userdata
  linkBackendCreate,
  linkBackendDestroy,
  linkBackendEnable,
  linkBackendDisable,
  linkBackendIsEnabled,
  linkBackendNumPeers,
  linkBackendCaptureAudioSessionState,
  linkBackendRequestBeatAtLaunch,
  linkBackendOnAudioFrame,
  linkBackendResetHostTimeFilter,
};

const LinkBackend* linkBackendRealGet(void) {
  return &kLinkBackend;
}

#else  // !DESKTOP_BUILD && !PORTMASTER_BUILD && !ANDROID_BUILD

// Keep translation units non-empty on stub platforms so pedantic builds
// that dislike TU-less objects stay quiet.
typedef int linkBackendRealUnusedType;

#endif

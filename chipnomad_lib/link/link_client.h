#ifndef __CHIPNOMAD_LIB__LINK_CLIENT_H__
#define __CHIPNOMAD_LIB__LINK_CLIENT_H__

#include <stdint.h>

// Receiver-only Ableton Link client. Mirrors the architectural shape of
// chipnomad_lib/midi/midi_router.{h,cpp}: a backend-abstracted client
// that the UI thread polls, with serial-bumped getters so callers apply
// each state change exactly once.
//
// RECEIVER-ONLY CONTRACT — ChooChooTracker never acts as a Link session
// host. This module MUST NOT call:
//   - ableton::Link::setTempo()
//   - ableton::Link::Client::setIsPlaying()
//   - ableton::Link::Client::commitAudioSessionState()
//   - ableton::Link::startPlaying() / stopPlaying()
// Allowed calls:
//   - enable() / disable() / isEnabled()
//   - captureAudioSessionState() (read-only, audio thread)
//   - sessionState.tempo() / beatAtTime() / phaseAtTime() / numPeers()
//   - sessionState.requestBeatAtTime() (LOCAL alignment only — does not
//     push to peers)
// User BPM edits while slaved are swallowed by the UI (BPM row is
// blurred); they are NOT propagated to the session.
//
// The header never includes <ableton/Link.hpp> and exposes no
// ableton:: type, so it compiles unchanged on platforms where Link is
// stubbed out (Web, Miyoo, RG35xx).

enum class LinkStatus : int {
  disabled   = 0,  // not selected as clock source, link disabled
  selected   = 1,  // clock source = Link, but Connect not pressed yet
  noNetwork  = 2,  // Connect pressed, no usable network interface
  connecting = 3,  // Connect pressed, mDNS discovery in flight
  noPeers    = 4,  // discovery running, no Link sessions found
  connected  = 5,  // 1+ peers, session tempo being applied
  error      = 6,  // Link library reported an internal failure
};

struct LinkClientState;

LinkClientState* linkClientCreate(void);
void linkClientDestroy(LinkClientState*);

// Selecting Link as clock source = a UI preference (no network event).
// Status transitions: any -> selected (or any -> disabled on Deselect).
// link.disable() is called if was previously enabled.
void linkClientSelect(LinkClientState*);
void linkClientDeselect(LinkClientState*);

// Pressing Connect = the actual network opt-in (honors privacy policy).
// Status transitions: selected -> connecting -> (noNetwork | noPeers | connected).
// On Connect: link.enable() is called; status transitions through
// connecting; if no network interface, go to noNetwork; if no peers
// within the discovery threshold, go to noPeers; if peers appear, go
// to connected.
void linkClientConnect(LinkClientState*);

// Pressing Disconnect = leave the session but stay selected as Link.
// Status transitions: any -> selected. link.disable() is called.
void linkClientDisconnect(LinkClientState*);

// Polling API (UI thread, mirrors midi_router's serial-bump pattern).
// getStatus returns the latest status; *serial bumps on every
// transition, so callers comparing serials apply each change exactly
// once. Returns 0 if status has never been observed.
LinkStatus linkClientGetStatus(const LinkClientState*, uint32_t* serial);
int        linkClientGetPeerCount(const LinkClientState*);  // 0 if not connected
float      linkClientGetSessionTempo(const LinkClientState*, uint32_t* serial);
// 0 if not connected

// Called from the audio callback at the tick boundary. Captures the
// latest session state (tempo, peers, phase). Realtime-safe per Link's
// contract. Returns 1 on success, 0 if Link is not enabled.
int linkClientCaptureAudioSessionState(LinkClientState*, double* tempo, uint64_t* peers);

// Called from the audio callback right before playbackStart with peers
// present. Captures the session state, calls
// sessionState.requestBeatAtTime(0, hostTime + outputLatencyMicros +
// schedulingHeadroomMicros, quantum), and returns the absolute host
// time (microseconds) at which playback should begin. If no peers,
// returns 0 (immediate start, per locked decision).
//
// NOTE: requestBeatAtTime takes THREE args (beat, hostTime, quantum) —
// not two. The research note's shorthand "(0, quantum)" was wrong.
uint64_t linkClientRequestBeatAtLaunch(LinkClientState*, int64_t quantum);

// --- UI-thread poll resolution ---
// Called once per UI frame (app.cpp). While a Connect is pending,
// resolves connecting -> connected when the backend reports peers, or
// connecting -> noPeers once the discovery threshold (3 s) has elapsed
// since Connect. No-op in every other state.
void linkClientPoll(LinkClientState*);

// --- Audio-thread plumbing ---
// Feed the backend's HostTimeFilter with one (sampleCounter,
// hostTimeMicros) pair from the audio callback entry (see T29). The
// sample counter is the cumulative count of samples processed since
// enable(); the host time comes from the same steady_clock scale as
// linkClientNowMicros().
void linkClientNotifyAudioFrame(LinkClientState*, uint64_t sampleCounter, uint64_t hostTimeMicros);

// Reset the HostTimeFilter window: on enable(), sample-rate change or
// buffer-size change.
void linkClientResetHostTimeFilter(LinkClientState*);

#endif // __CHIPNOMAD_LIB__LINK_CLIENT_H__

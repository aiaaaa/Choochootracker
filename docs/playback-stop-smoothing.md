# Playback stop and mute/solo smoothing

Stopping a sounding song could jump abruptly to silence across engine families.
The shared callback also discarded valid audio whenever the renderer ended partway
through a block. SoundFont's existing note-cut smoothing did not cover that
transport boundary.

The renderer now preserves its last stereo output and removes the stop discontinuity
with a three-millisecond output correction. It finishes that correction across
callback boundaries, keeps idle output silent, and lets the next queued start run
at a tick boundary. Ordinary playback takes a constant-time history update per
render chunk; smoothing allocates no memory and retains no sample pointers.
SoundFont voice tails are cleared once the shared output owns the stop fade.
The callback preserves valid partial blocks instead of replacing them with silence.

Mute, unmute, solo, unsolo and group changes use the same output correction when
the audio thread applies a changed enabled-track mask. Both directions are covered
without stopping playback or changing voice envelopes. Repeated settings and
changes to unused tracks do not restart the correction. Rapid toggles start from
the current corrected output; changes while idle do not manufacture a fade.

Fixed selection exports retain their exact requested length. StopRange boundaries
do not append a transport fade. This change does not alter preset envelopes,
samples, sustain, release, or charging settings.

Regression coverage includes SoundFont, OPLL, DX7, MONK, Braids, Sega PSG and AY,
callback partitioning at multiple sample rates, silence after stop, restarting,
SoundFont preview retirement and fixed selection export lengths. Mute/solo coverage
also checks group changes, switching solos, rapid toggles during a correction,
unchanged masks, idle changes and convergence to the normal mix. Physical listening
on the target device remains an owner check after installation.

Keep this change separate from collection browser work for a future contribution.
No pull request has been submitted.

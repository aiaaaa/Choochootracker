# October 11 personal integration

Upstream snapshot: `46f76a1` (Fix Insert FX navigation test state).
Both R36H and web compile from `personal/r36h`. Main and the upstream repository
remain unchanged. Only work merged into upstream main is integrated.

Included PRs and maintainer follow-ups:

- #38: Note Lock, phrase fills/randomization, EQUAL/AUTO/LAZY sample slicing,
  slice playback/preview, WAV cue points and related controls.
- #51: Rotary speed and contextual insert editing/Shift navigation, replacing
  the earlier personal implementation with the accepted version.
- #52: community instrument import through USER banks; private bank files stay
  outside Git and public web packaging.
- #53: M8 instrument conversion and Braids/Sample M8 export.
- #54: font-generator build fix.
- #56: FM operator editor for Genesis, Arcade, OPL2/3 and OPLL/VRC7.
- #57: correct HiDPI browser touch coordinates.
- Main follow-ups: stereo pan/mixer monitoring, PortMaster rendering and relative
  file paths, lower idle CPU load, configurable audio buffers, master inserts,
  parametric EQ, envelope fixes, FM catalog fixes and CI/navigation corrections.

Merge decisions:

- Keep upstream project format 7 for plain projects so slice-loop migration is
  not repeated on reload; native chip formats retain upstream 9/10 handling.
- Keep Lucky in the new Settings structure, after Support report and before
  Quit. Key Mapping now lives under Controls. Keep browser hide/wake recovery,
  immediate settings flush, theme integration and X/Y defaults.
- Historical first integration retained a personal 4906-frame buffer choice.
  This was removed in `88f8c13` after the owner confirmed 256 worked well.
  Current choices and normalization follow upstream: 32/64/128/256/512/1024.
- Use upstream's PortMaster O3 optimization and frame timing with Lucky linkage.
- Keep generated web/dist ignored and require a fresh exact-commit CI build.
- Carry forward roughdemos, browser-safe editable seeding and the separately
  named device Continue snapshot. Never overwrite existing browser projects.

Validation uses the native test suite, isolated browser presentation/Lucky/song
checks, an ARM64 build and UI checks, package integrity, installed-data hashes
and public deployment verification. Physical iOS controller/Bluetooth behavior
and subjective R36H playback require user evaluation. Build/install receipts
record the actual source SHA, tests completed and rollback location; this file
is the integration scope, not a claim that pending checks have passed.

Local validation of the merged application source (`b1d562f`):

- Native suite: 627 cases and 112,827,440 assertions passed; stretch lifetime
  regression passed with no retained allocations.
- Personal Lucky/Settings checks: 26 cases and 10,270 assertions passed. The
  Settings test follows the new upstream row positions. The M8 sample lookup
  test accepts the resolved capitalization on case-insensitive filesystems.
- Fresh Wasm build: all 11 runtime files and packaged source assets validated.
- Isolated Chrome: theme and mapping persistence, responsive layout, startup
  audio activation, simulated background/wake/output recovery, reload data
  preservation, synthetic gamepad input and Lucky playback/import passed.
- All 17 roughdemos loaded and round-tripped with their embedded sample bytes
  intact; existing edits, Continue and private preset data survived reload.
- Web workflow lint and all five Lucky relay tests passed.

The fresh R36H binary is built from `b1d562f`; subsequent test/documentation
corrections do not change its application source. Installation writes the exact
binary hash, validation results and rollback path into `personal-build.json`
and `personal-device-validation.json` in the device installation. The public
web build's exact source commit is recorded in `/build-info.json`.

Buffer choices follow upstream Settings → Audio. The owner confirmed playback
at 256 frames. Browser and Bluetooth output can add their own latency.

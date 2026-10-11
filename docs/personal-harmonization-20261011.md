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
- Keep the saved R36H buffer of 4906; expose it as an additional personal
  handheld Audio choice rather than silently converting it to 512.
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

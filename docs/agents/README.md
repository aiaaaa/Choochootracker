# Agent documentation index

These files are the context pack for coding agents working on ChooChooTracker.
They exist so an agent can load only the layer it is about to change, then
append caveats when reality disagrees with the docs.

Human-facing product docs stay in `docs/USER_MANUAL.md`, `docs/dev_readme.md`,
and `docs/build-notes.md`. Do not replace those with this pack.

## Load this first

1. This index.
2. `docs/agents/DOC_PROTOCOL.md` if you will edit code or docs.
3. Exactly one domain doc from the table below. Load a second only if the
   change crosses that boundary.

## Domain docs

| Task | Read |
| --- | --- |
| Sequencer, project format, ownership, threads, adding a module | `docs/agents/CORE_ARCHITECTURE.md` |
| PortMaster zip, launch script, gptokeyb, ArkOS paths, ARM64 package | `docs/agents/PORTMASTER.md` |
| RG353V / handheld Linux limits, CPU, storage, input, screen | `docs/agents/HANDHELD_LINUX.md` |
| Screens, 40x20 grid, colors, navigation, instrument forms | `docs/agents/UI.md` |
| Callback, voices, engines, mix, reverb/delay, sample rates | `docs/agents/AUDIO.md` |

## Existing deep references

Use these after the agent doc, not instead of it:

| Topic | File |
| --- | --- |
| Short architecture overview | `docs/ARCHITECTURE.md` |
| UI/audio thread contract | `docs/realtime-architecture.md` |
| Review paths | `docs/codebase-map.md` |
| Build commands | `docs/build-notes.md` |
| Engine/DSP notes | `docs/development-notes.md` |
| Product goals and roadmap | `docs/dev_readme.md` |
| Upstream ChipNomad policy | `docs/fork-maintenance.md` |
| 48 kHz / CPU measurements | `docs/performance-optimization-2026-09-01.md` |
| User-visible behaviour | `docs/USER_MANUAL.md` |
| ASCII layout sketches | `docs/designbook/README.md` |

## Hard product facts

- Name: ChooChooTracker. Format: `.cct`. Eight fixed monophonic tracks.
- Fork of ChipNomad. Compatibility with ChipNomad project files is not a goal.
- Primary hardware target: Anbernic RG353V through PortMaster (ARM64).
- Shared engine for Windows, Web, Android, PortMaster. Platform code lives in
  `tracker/platforms/`.
- UI owns `Project`. Audio reads `audioProject`. Never write the project from
  the audio callback.
- Master mix: 48 kHz float stereo. Default callback buffer is large; do not
  shrink it without a hardware check.

## Do not load unless needed

- `chipnomad_lib/external/mutable/` is frozen DSP. Read wrappers in
  `chipnomad_lib/synth/` first.
- `docs/feasibility-2026-08-09.md` is historical. Some sample-rate assumptions
  there are stale (mix is 48 kHz, not 96 kHz).
- `tracker/packaging/portmaster/README.md` still mentions 96 kHz. Code and
  `docs/agents/AUDIO.md` are the source of truth.

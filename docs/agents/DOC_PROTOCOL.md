# Agent documentation protocol

Purpose: keep layer docs small, true, and useful as LLM context. Agents must
read the matching doc before changing that layer, and must write findings back
when the code or hardware surprises them.

## When to read

Before editing, match the change to `docs/agents/README.md` and read that
domain doc. Also read this protocol if you will add a caveat or a new rule.

Skip the full pack. Context is a budget. One domain doc plus the files you
will touch is the default.

## What belongs in these docs

Record facts that another agent would otherwise rediscover the hard way:

- ownership and thread boundaries
- layout, naming, and control conventions
- hardware or PortMaster constraints
- non-obvious order of operations
- IDs that must stay stable (FX enums, instrument types, Plaits engines)
- measured limits (CPU, buffer size, glibc, screen)

Do not copy the user manual. Do not paste large source listings. Point to
paths and symbols instead.

## How to update a domain doc

Prefer a surgical edit:

1. If a rule is wrong, fix the rule in place. Keep the wording short.
2. If you found a limitation, crash, or non-trivial workaround, append one
   entry to that doc's `Caveats log`.
3. If a whole subsystem is new, add a short section with source-of-truth
   files, then a caveat if validation is incomplete.
4. Do not rewrite the document for style. Do not duplicate the same caveat
   in every file. Put it in the layer that owns the constraint, and add a
   one-line pointer elsewhere only if another layer will hit it.

Never update in-app help. Before an alpha commit, update `docs/USER_MANUAL.md`
for user-visible behaviour. Keep `AGENTS.md` build commands in sync with
`docs/build-notes.md`.

## Caveat entry format

Append at the bottom of the matching domain doc. Newest last. Use this shape:

```text
### YYYY-MM-DD — short title

- Symptom: what broke or looked wrong
- Cause: the actual constraint or bug
- Rule: what agents must do or avoid
- Files: path:symbol if known
- Status: open | workaround | fixed
```

Example:

```text
### 2026-09-01 — 96 kHz mix overruns RG353V

- Symptom: crackles and UI stalls on dense songs
- Cause: master mix and effects ran at 96 kHz
- Rule: keep master mix at 48 kHz; Braids may stay 96 kHz internally
- Files: chipnomad_lib/synth/braids_voice.h, docs/performance-optimization-2026-09-01.md
- Status: fixed
```

If the issue is fixed, keep the entry. Future agents need the history more
than a clean page.

## Rules for vibe-coding sessions

- Name the layer you are in before coding: UI, engine, PortMaster, handheld.
- Touch the smallest set of files that preserves existing conventions.
- After a non-trivial surprise, update the caveat log in the same change.
- If you are unsure whether a behaviour is intentional, treat the current
  hardware target (RG353V / PortMaster) as the tie-breaker.
- Do not invent parallel systems. Reuse ChipNomad tracker structure, the
  three FX columns, `InstrumentDefinition`, and `VoicePostProcessor`.
- Do not "clean up" Mutable Instruments snapshots, FX enum order, or
  project field layout while adding a feature.

## Validation after doc-worthy changes

From `tracker`:

```text
make -f Makefile.test -j4
```

Audio or engine changes also need a listen on desktop. PortMaster, input,
timing, and CPU claims are not proven until an RG353V run.

## Anti-patterns

- Loading every markdown file in `docs/` into context
- Writing a new architecture essay instead of editing the domain doc
- Putting hardware notes only in a chat transcript
- Changing UI coordinates without reading `docs/agents/UI.md`
- Adding an engine without reading `docs/agents/AUDIO.md` and
  `docs/agents/CORE_ARCHITECTURE.md`

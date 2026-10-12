# User manual translations

Translations of [`../USER_MANUAL.md`](../USER_MANUAL.md). The English manual is
the reference: when a translation and the English text disagree, the English
text is right.

| Language | File | Translated from |
| --- | --- | --- |
| Français | [`USER_MANUAL.fr.md`](USER_MANUAL.fr.md) | English manual at `0bffff9` |
| Русский | [`USER_MANUAL.ru.md`](USER_MANUAL.ru.md) | English manual at `0bffff9` |

## Conventions

- The app interface is in English, so screen, menu, field and button names
  (Song, Chain, Phrase, Instrument, Table, Mixer, **Settings**, **EDIT**,
  **OPT**, **SELECT**...), FX codes (`HOP`, `TBX`, `M1A`...), module and engine
  names (ADSR, LFO, Braids, Plaits...) and file extensions are not translated.
- Same structure as the English manual: same headings in the same order, same
  tables, code blocks and links. Keep it so a diff against the English file stays
  easy to follow.
- No em dash: use a plain `-`.

## Keeping a translation current

Each file says which English commit it follows. To update one, diff the English
manual from that commit (`git diff <commit> -- docs/USER_MANUAL.md`), translate
the changed passages, and bump the commit in the file header and in the table
above.

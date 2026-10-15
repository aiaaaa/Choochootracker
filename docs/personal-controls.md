# Device personal build controls

These additions supplement the [upstream user manual](USER_MANUAL.md).
See [the personal feature record](../PERSONAL_FORK.md) for integration history.
Accepted synth and insert features use the main manual; the notes below retain
personal experiments and platform-specific behavior.

### Personal opt-in module discovery

Builds made with `CHOOCHOO_EXPERIMENTAL_MOD_LUCKY=1` add one Settings row,
**I'm Feeling Lucky — NEXT PLAY LOAD**, immediately above Quit. Left/Right
selects an action and the existing EDIT control activates it. NEXT prepares a
random module with an animated underline; then release EDIT and activate PLAY
to listen. LOAD imports the exact prepared module’s embedded PCM sample bank.
Fetching requires working internet access on the handheld; a USB SSH connection
alone does not supply internet access.
The ordinary native build omits this experimental row. The personal web build
also includes Lucky: NEXT fetches through the site and archive, PLAY requires
browser audio activation, and LOAD stores samples privately in this browser.
Leaving Settings cancels an in-progress download. Keep downloaded project
backups; browser storage can be cleared or evicted. See
[the experiment notes](mod-lucky.md) for build instructions, persistence,
format/loop limitations and validation status.


### Personal fork integration (October 7, 2026)

This build follows upstream 1.0.15, retaining Factory/USER collections, ZIP packs
and continuous USER preset stepping. In personal Settings, Support report is
followed by Get Lucky and Quit. Opt returns to Song. Vita development is paused
and is excluded from this update.

### Personal browser presentation

The web page follows the active tracker theme and keeps its title, file controls,
status and links below the canvas, followed by the touch controller. Theme changes are saved to
this browser automatically. The viewing area uses the original fit-to-width
presentation, up to 640 pixels wide with a 4:3 aspect ratio. There are no custom
integer or font-dependent scaling steps. Controls can scroll below the viewport.
The native tracker layout and controls are unchanged.

When the browser page is hidden or the phone locks, the web build saves the
current project/settings and stops transport. Returning and tapping a control
attempts to resume browser audio; press Play to continue in the same project. Keep downloaded
backups because the browser can still terminate a page or clear its storage.

Safari's Share → Add to Home Screen uses the train-with-headphones icon and the
name ChooChoo. Home Screen launch uses a standalone window. Opening/reloading
still requires a network connection; this does not add offline installation.
Export projects before switching between a Safari tab and a Home Screen install,
which may have separate browser storage.

New browser settings use **PAD X** for Play and **PAD Y** for Shift, alongside
keyboard Space and Shift. Saved mappings override these defaults; existing
installations can select X/Y in Key Mapping without clearing their data.
Native controller defaults are unchanged.

After changing browser controls in Key Mapping, choose **Done**. Settings saves
immediately request a browser-storage flush, so they do not wait for the periodic
save timer. Custom mappings belong to that browser/Home Screen installation.

The web action buttons use compact uppercase labels and wrap to the available
width. Project/app actions precede the sample and preset imports.

The personal web app includes **[rough demos]** under **Project → Load** in
`/projects`: device songs plus a separately named Blue Hour Continue snapshot.
Sample-based songs include their WAVs. Bundled demos refresh on reload. Old,
unchanged demo copies are removed from `/user/projects/rough demos` and the older
`roughdemos` folder; edited or extra files stay intact. To keep your own version,
save it under `/user/projects` or download its `.cct`.

**Continue** is one rolling autosave at `/user/autosave.cct`, replaced by the
current project as you work. It is saved roughly once a minute while active,
when the page is hidden, and before **Reload app**. It is not a history of songs.
Imported projects and named saves in `/user/projects` are separate files; you
can keep multiple songs there. `/user/samples` holds imported samples,
`/user/instruments` holds imported presets, and `/user/exports` holds exports.
These are virtual folders stored in this website's browser database on this
phone, not folders in the phone's Files app or cloud storage. They normally
survive reloads; clearing site data can remove them. **Download .cct** creates
a portable file you can keep outside the app.

The Home Screen app respects the camera and Home indicator safe areas, with
an extra 25 pixels above the tracker. Initial audio waits quietly for your first
touch or key press; opening the app does not require Restart audio. Use
**Reload app** below the tracker when there is no Safari refresh button; it saves
and flushes this browser's storage first, and cancels if saving fails.
**Restart audio** reopens the web audio output while keeping the current project
and loaded samples. Select Bluetooth output in iPhone Control Center first.
If sound stops after locking/unlocking, tap a control, then try Restart audio.
Physical iOS recovery is still being tested; keep downloaded project backups.


## Experimental SQ80/ESQ (personal build)

Choose SYNTH → SQ80/ESQ, import your wave bank and presets into USER/sq80esq,
and open EDIT PRESET for the full native editor. Web users can use the
Import SQ80/ESQ button once; files persist locally. See [SQ80/ESQ](sq80esq.md)
for controls, bank formats, playback indicators and current limits.
The SQ80/ESQ home page uses uppercase **EDIT**, with the same theme action
color and selected-value highlight as other instrument editors.


### SQ80/ESQ Rev2 performance controls

The experimental SQ80/ESQ instrument now provides Wheel, Velocity, Pedal,
FACTORY/USER selection and Cutoff/Resonance on its main page. Preset browsing
keeps Velocity and resets Wheel/Pedal. Waves and Find wave banks are at the top
of EDIT PRESET; Profile is hidden and saved profiles are preserved.
Roll controls are QCF (Cutoff), QRS (Resonance), WHL (Wheel), PED (Pedal),
SUS (Sustain), AFT (track aftertouch), NPR (latest-note pressure), LFR (LFO1 rate),
GLI (Glide time), VEL (Velocity), and EXT (External). No additional ADSR is added.
See [SQ80/ESQ instructions](sq80esq.md) for ranges, persistence and chord targeting.

USER preset browsing uses two panes: collections on the left and their presets
on the right. Left/Right switches panes, Up/Down moves within a pane, EDIT selects,
and EDIT+PLAY auditions. The collection path remains visible below both panes.
Folder and ZIP collections are grouped without extracting presets. FACTORY
browsing keeps its existing layout. Reopening USER recalls the selected sound.

### SQ80/ESQ Rev3 browsing

Bank sits above Preset, with EDIT beside the preset name. Hold EDIT + Left/Right
on Bank to switch FACTORY/USER; on Preset it continues to step through sounds.
Release EDIT alone to open the chooser/browser. Using an EDIT chord suppresses
the release popup. Select the adjacent EDIT control to open patch parameters.

### SQ80/ESQ Rev4 alignment

The patch editor control reads **Edit** and aligns directly above the word
**Velocity**. The preset row shows up to nine name characters beside it; the full
instrument name remains in the header. Browsing and editor controls are unchanged.

### October 13 personal daily compatibility

Settings touch controls include Lucky NEXT, PLAY and LOAD at their displayed
positions. Quit remains a separate row. Lucky stays opt-in in native builds and
enabled in both personal targets. The Android working-folder control can coexist
with Lucky.

The personal web **Import project** button accepts `.cct`, `.lsdsng` and `.sav`.
For an LSDj save, choose a song in the tracker popup; cancel keeps your current
project. Upstream LSDj export is available on desktop and Android, not on web or
PortMaster. The Modulation insert group keeps Chorus, Phaser and Flanger;
personal Beads remains available in its own group.

### October 14 combined personal daily

Instrument motion destinations and FX lists preserve the wider lane FX identifiers,
including the empty destination marker, across the upstream refactor.
Short imported VT2 patterns also retain their end-of-pattern skipping behavior.

This daily includes the upstream v1.0.16-alpha changes and subsequent lane FX
refactor, plus the revised SID home controls and SQ80 EDIT styling. Phrase and
Table FX slot counts and 00–FF value editing are unchanged. Trance Gate uses the
stock module number; old personal Beads songs are converted with the update,
with originals retained for rollback. User presets and settings are preserved.

### Combined SoundFont and SONG waveform evaluation

The personal evaluation combines the SID home layout with the SONG-only waveform
and experimental SoundFont instrument. The saved Song waveform preference is
retained; Instrument and other editors use their full layouts. ESQ stays experimental
and is excluded from the planned SID layout contribution.

Use SAMPLE → SOUNDFONT → Preset; Rescan refreshes the collection list. Sound banks remain
external user data under instruments/USER/soundfont; web adds Import sound banks and Import bank
folder. Existing SQ80 and Monk instrument identities remain 28 and 29; SoundFont
uses 30 in this personal evaluation. Keep the original bank beside saved projects.
See docs/soundbanks/validation.md for format and memory limits.

Browser **Import USER ZIP** accepts a ZIP rooted at `instruments/USER/` or
`USER/`, including `soundfont` banks and their relative sample folders. Existing
files are kept. Import only the collection you need: each import supports up to
1 GiB of new uncompressed files, with a 512 MiB per-file limit.

Browser audio recovery waits for clock progress and a real output callback within
a bounded timeout before reporting success or failure.

SoundFont status and compatibility warnings stay on its Instrument page.


### SoundFont evaluation daily follow-up

This personal daily includes the expanded SoundFont Edit page and its eight
Lane FX. Use Rescan after changing USER/soundfont, select presets through their
folders, or hold EDIT with Left/Right on Preset to cycle. The full controls and
FX value ranges are documented in the SOUNDFONT section above.

# Personal web deployment

## Architecture and audit

The personal branch uses the existing C++ tracker, SDL2 browser platform and
`tracker/Makefile.web`; there is no separate frontend. Emscripten compiles shared
sources from `Makefile.common`, replaces the native main loop with
`tracker/platforms/web/corelib_mainloop.cpp`, and enables `WEB_BUILD`, gamepads,
Asyncify, exceptions, growing memory (256 MB initial), and IDBFS. SDL2 is built
by Emscripten's port system; the first build needs network access. Synth cores
are already vendored. No Docker, native ALSA/RtMidi, or UI framework is needed.

Before this change, `web/dist` was committed. Root `vercel.json` performed only
an echo and published it, permitting `main` and disabling other branches. The
old Web action ran on tags/manual dispatch, rebuilt the bundle, then rebased and
pushed it to `main`. A recent binary commit was not proof that every later source
change was compiled. The checked-in manual also had no clean-build copy step.

Now the **single Web action** compiles the exact triggering SHA on
`personal/r36h`, validates `web/dist`, uploads a SHA-named artifact, then uses
Vercel CLI's `pull`, `build`, and `deploy --prebuilt`. Vercel's local build step
only validates and packages the fresh files; Vercel hosting does not compile
C++. The same checkout supplies the fixed-purpose `api/lucky.js` Node function.
Its packaged source must match byte-for-byte before upload; its source hash is
in the public manifest. The static output is checked again before upload. Native workflows and
native build settings are unchanged; CI's old checked-in-HTML comparison now
checks the browser source instead. `main` remains the upstream mirror.

`tools/web_bundle.py build` deletes only generated web output before compilation,
rejects untracked packaging inputs (including ignored private USER banks), checks
Wasm magic, browser exports, every preloaded asset byte and the package index,
and writes `build-info.json` with SHA, dirty status, SDK version and file hashes.
Failed compilation/packaging removes the incomplete output. Deployment requires
a clean source tree and matching SHA/hashes. SDK downloads are cached; app bundles
are never restored from cache. Serialized deployment and a branch-head check
prevent queued old commits from replacing a newer production build. After upload,
the public manifest check retries temporary HTTP errors and old manifests while
the production alias propagates, then fails if the exact artifact is not served.

Published files are `index.html`, `privacy.html`, `manual.html`, Emscripten's
`choochootracker.html`, `.js`, `.wasm`, `.data`, `lucky-license.txt`, and
`build-info.json`, `favicon.png`, `apple-touch-icon.png`, and `site.webmanifest`. `/api/lucky` is the only server function. Fonts,
themes, public factory presets, licenses, samples, waveforms, title art and the
Makefile's selected example projects live inside `.data`, mounted in the virtual
filesystem. The existing manual is now source-controlled at `web/manual.html`;
keep it current when the user manual changes. Generated `web/dist` is ignored.

## One-time account setup

1. Commit/push these changes to `aiaaaa/Choochootracker` on `personal/r36h`.
   Do not merge them into `main`. The first Web run can build/upload its artifact
   before credentials exist; its deploy step will clearly fail until configured.
2. In your own Vercel account, create a separate project, using framework
   **Other**, repository root **`.`**, output **`web/dist`**, empty install command,
   and build command **`python3 tools/web_bundle.py validate --require-clean`**.
   Import/connect the **fork**, never the upstream repository/project. Select
   **`personal/r36h`** in Settings → Environments → Production → Branch Tracking
   (called Production Branch in older dashboard layouts).
3. **Disconnect the project's Git integration after selecting the branch.**
   Actions is the sole publisher. This is necessary because upstream `main` and
   old contribution branches retain their own deployment-enabled configurations.
   The personal branch also sets `git.deploymentEnabled: false`; that disables
   Git-triggered builds, not authenticated CLI deployments. A Git import's initial
   build may fail because generated output is deliberately absent; do not deploy
   old committed files to work around it. Alternatively create/link an empty
   project with `vercel link` and keep Git disconnected from the start. With no
   Git integration, the workflow's branch check is the production authority.
4. Find the new project's Project ID and your team/account ID in Vercel settings
   (or `.vercel/project.json` after `vercel link`). Create a Vercel token scoped to
   that account/team. In the fork's Settings → Secrets and variables → Actions,
   add repository secrets **`VERCEL_TOKEN`**, **`VERCEL_ORG_ID`**, and
   **`VERCEL_PROJECT_ID`**. Do not paste tokens into source, issues, or build logs.
   No runtime environment variables are required. Do not enable automatic system
   variable exposure for this prebuilt static application.
5. Allow public access to **production** in Vercel's Deployment Protection
   settings if needed. The workflow verifies the public deployment URL without
   credentials. Preview protection may remain enabled. Enable GitHub Actions for
   the fork and allow the existing checkout/setup-emsdk/upload-artifact actions
   plus setup-node.
6. Trigger the workflow with `target=production` on `personal/r36h`. Check its
   summary URL and open the tracker. Compare `/build-info.json` with the workflow
   SHA. Only then consider the first public deployment complete.

The repository intentionally contains no personal domain, project ID or token.
Root `vercel.json` is authoritative; `web/vercel.json` fails with instructions if
someone incorrectly chooses `web` as the project root. Unversioned runtime
filenames use revalidation (`Cache-Control: public, max-age=0, must-revalidate`).
Do not add another Git hook, bundle-commit bot or deployment workflow.

## Everyday operation

- Push to `personal/r36h`: fresh production build/deployment automatically.
- Manual rebuild: Actions → Web → Run workflow → branch `personal/r36h` →
  `production`. GitHub may only show the dispatch button when the workflow also
  exists on the default branch; if absent, use the API/CLI command below. The
  pre-existing Web workflow on `main` already supplies the dispatch registration.
- Preview: same action with `target=preview`; it compiles the selected personal
  commit without moving the production domain. `target=build` uploads only an
  artifact and needs no Vercel secrets. Other branches and tags cannot publish
  through this personal workflow.
- After an upstream merge into the personal branch, push normally. Review any
  merge conflicts in the web workflow/Makefile/config; keep the clean-build
  pipeline. Never accept upstream's old bundle-commit-to-main step. The shared
  source list and asset discovery automatically include upstream changes.
- Inspect Actions → Web → compile/deploy logs for failures. The exact bundle is
  the `choochootracker-web-<SHA>` artifact. Vercel's deployment metadata and public
  `/build-info.json` identify the source commit. A build artifact alone is not a
  successful deployment; missing secrets/protection failures remain red.
- Roll back using Vercel's deployment history → select a known-good deployment →
  Instant Rollback, where available. Alternatively revert the bad source change
  on `personal/r36h` and push; this builds a new production artifact. A subsequent
  push will replace a dashboard rollback. Export songs before testing changes.

With GitHub CLI authenticated, dispatch explicitly with:

```sh
gh workflow run web.yml --repo aiaaaa/Choochootracker \
  --ref personal/r36h -f target=production
```

### Local build

Use Python 3.9+, GNU make, and **Emscripten 6.0.10** (the CI pin). Activate an
existing SDK with `source /path/to/emsdk/emsdk_env.sh`; Windows SDK users should
follow the PowerShell activation notes in `build-notes.md`. On the audited Mac,
the existing SDK is in the parent workspace's `.tmp/emsdk`, not this checkout.

```sh
MOD_LUCKY_DEPS_DIR="$PWD/.tmp/mod-lucky-web" \
  emconfigure scripts/build-mod-lucky-dependency.sh --host=wasm32-unknown-emscripten
python3 tools/web_bundle.py build
python3 tools/web_bundle.py validate
python3 -m http.server 8080 --directory web/dist
```

Open `http://localhost:8080`. This static server supports the tracker but not
Lucky NEXT: the latter needs the `/api/lucky` function, available on Vercel.
The web target uses the existing SHA-256-pinned libxmp-lite 4.7.3 dependency
script, compiled with Emscripten into an isolated `.tmp/mod-lucky-web` prefix.
Initial dependency builds need HTTPS access to GitHub; no native libcurl or
threads are linked into Wasm. Do not reuse the native/ARM prefix. A local dirty build is marked `dirty: true` and
cannot pass deployment's `--require-clean` check. `EMXX` may specify an executable
path. A clean checkout must not contain private USER banks. Do not copy handheld
assets into tracked packaging directories to prepare a web build.

## Browser behavior and personal features

The personal web defaults are Departure Mono and persistent waveform ON.
An older browser profile receives those two appearance choices once, marked by
`webPersonalAppearance: 1` in its settings. Existing key mappings, colors and
other settings are preserved. After that migration, manual appearance changes
remain saved across deployments. Native defaults are unchanged.

The build retains shared personal fonts/themes, piano and mixer meters,
waveform choices, native chip engines and factory banks, insert effects,
rotary-rate/popup navigation, preset collection/import code and wavetable fixes.
It does not package the device's private collections. R36H/PortMaster hardware
and native MIDI backends are already platform-guarded. The personal web target
enables Mod Lucky: asynchronous Emscripten Fetch replaces native libcurl, and
bounded decoding runs during Settings updates using the same libxmp parser,
preview buffer and sample-bank importer. Native worker/audio behavior remains
unchanged. NEXT uses `/api/lucky` for the archive random-player page because that
page lacks CORS; module bytes download directly from the archive's CORS-enabled
endpoint. The relay accepts only GET with no parameters and one fixed upstream
URL, follows no redirects, caps HTML at 256 KiB and respects archive errors and
Retry-After. No extra credentials/environment variables are required. Hosting
function quotas and archive availability can limit discovery.

PLAY requires the normal browser audio activation. LOAD writes samples and
source attribution into `/user/samples/mod_lucky`; existing IDBFS synchronization
persists them locally. It does not upload or publicly redistribute imported
modules. No song starts automatically after NEXT. Background-tab throttling can
interrupt previews; foreground playback is the tested path.

The wrapper currently auto-loads the tracker and hides Start. SDL/Emscripten
creates Web Audio; its SDL2 browser backend resumes initial suspended audio
after user activation (with gesture listeners as an older-browser fallback). Tap the canvas or a control after loading to activate sound on iOS;
a page load alone cannot bypass browser gesture rules. iPhone Silent Mode may
still affect audible playback. When the page is hidden, the web adapter sends the existing sleep event to save
the current project/settings and stop transport, clears held input, and requests
an IDBFS flush. Returning wakes SDL audio; visibility and user-interaction
listeners resume suspended or interrupted Web Audio. The project remains open;
press Play to continue. A browser/OS can still kill the page or reject audio
activation, so physical iOS lock/unlock testing remains necessary. Native audio
and rendering behavior are unchanged.

Keyboard controls, pointer-based on-screen gamepad and browser gamepad polling
remain unchanged. The wrapper imports `.cct` and WAV files/folders and downloads
projects. IDBFS mounts `/user`, synchronizes every 10 seconds and on `pagehide`;
file imports and downloads also request a sync. Browser storage can be denied,
evicted, cleared or lost in private mode; pagehide is not a durability guarantee.
Settings/autosave and imported projects use the existing web paths. Public
factory assets outside `/user` are rebuilt in memory at each load. Keep downloaded
project backups. **Import presets** accepts an `instruments` folder containing
`USER/<engine>/...` (and optionally `banks/<engine>/...`), or the `USER` folder
itself. Files stay in this browser's `/user/instruments`, backed by IDBFS; they
are never uploaded to Vercel or included in the public bundle. ZIP banks stay
zipped and collection subfolders are preserved. Existing paths are kept rather
than overwritten; choose the folder again to complete an interrupted import.
Hidden files, unknown engine folders and FACTORY are skipped. Reopen an
instrument's Bank → USER to refresh the list; ALL uses the same imported library.
Keep the original library: changing browser/domain or clearing site storage
requires reimport. Mobile folder-picker support varies; import on each browser
where you want the library. Native USER paths and defaults are unchanged.

## Browser presentation

The canvas is the first page element. File buttons, status, title
and links follow it, with the touch controller last. `web/index.html` controls that layout and its CSS palette.
The web-only main loop forwards the actual active tracker colors when they change;
the wrapper updates CSS variables. Theme loads, resets and direct color edits also
save settings and request an IDBFS flush, so a tab reload does not depend on the
native Quit path. This does not change native settings behavior.

The backing canvas remains 640×480; automatic SDL high-DPI enlargement is
disabled only for the web target to avoid a second scaling step. The page chooses
the largest scale that fits in display pixels, accounting for `devicePixelRatio`.
The web adapter checks all 95 glyph bitmaps of the active font. When every glyph
consists of doubled 2×2 pixels (including its padding), it permits half canvas
steps: those still give each original font pixel an integer number of display
pixels. Departure Mono supports this; other fonts fall back to whole canvas steps.
Changing fonts updates the scale automatically. This is not arbitrary fit-to-width
scaling: on an iPhone 15 Pro's normal 393-pixel/3× portrait viewport, Departure Mono
uses 320×240 CSS pixels, with each original font pixel 3×3 display pixels. That is
50% larger than the previous 213⅓×160 view without cropping or sideways scrolling.

The left edge is snapped to the display pixel grid. Controls may extend below the
viewport. Single-pixel graphics such as waveform lines can have alternating widths
at a half canvas step; this guarantee concerns the doubled font, not every graphic.
The small title-screen text uses a separate reduced font. No horizontal scrolling
or larger-view toggle is added. Resizing, orientation changes and moving between
displays recompute the fit. Browser/OS compositing can still affect appearance;
physical Safari and iPhone checks remain useful. Native rendering is unchanged.

## Home Screen layout and recovery

The viewport uses `viewport-fit=cover`; main content adds the four CSS
`env(safe-area-inset-*)` values to its normal margins. Canvas sizing subtracts
that vertical padding. Standalone Home Screen windows add another 15 CSS pixels
above the tracker for iOS UI clearance; ordinary browser tabs keep their existing
spacing. This keeps the tracker and controls out of the camera
and Home indicator areas in a standalone Home Screen launch.

**Reload app** pauses input, saves the current project and settings, and waits
for the existing IDBFS flush before reloading. Serialization/storage errors or
a 10-second timeout cancel the reload and leave the working project open.
It does not clear site storage. Keep downloaded backups independently.

Initial audio suspension while waiting for a touch/key gesture is normal and
does not display a recovery warning. Focus and page-show events only attempt
recovery after audio output has actually run. The first interaction activates
the existing output; Restart audio is not required for ordinary startup.

Browser audio recovery checks both the AudioContext clock and SDL's output
callbacks, since a reported `running` state alone is insufficient on some Safari
versions. Returning from the background or a user gesture can perform a bounded
suspend/resume cycle. **Restart audio** explicitly closes/reopens only SDL's web
output using the same callback, rate and buffer request; synth/project/sample
state stays in memory. Reopening also gives the browser a new output context
after a device-route change. Choose Bluetooth speakers in the iPhone's Control
Center output picker; the app does not select hardware routes.
The passive health check never repeatedly resumes audio, sets a wake lock, or
adds silent media. Auto-Lock and physical output routing remain controlled by iOS.

These are recovery paths, not a claim that all Safari audio interruptions are
fixed. Physical iPhone Home Screen, Auto-Lock and Bluetooth transitions need
retesting. Some browser failures can still require a saved reload or relaunch.
Background save/flush is best effort if iOS immediately terminates the process.
Relevant platform notes: [WebKit safe areas](https://webkit.org/blog/7929/designing-websites-for-iphone-x/),
[WebKit frozen audio clocks](https://bugs.webkit.org/show_bug.cgi?id=263627), and
[Apple Bluetooth output selection](https://support.apple.com/en-gb/guide/iphone/iph01b6ea9a3/ios).

## Browser and Home Screen icons

`web/favicon.png` and `web/apple-touch-icon.png` reuse the supplied flat
`train_headphones` artwork, unchanged at 128×128 and 1254×1254 respectively.
The larger original avoids enlarging the small favicon; iOS selects and scales
the linked Apple touch icon. `web/site.webmanifest` defines the name, relative
start URL and standalone launch mode. The Makefile copies these with the wrapper;
bundle validation checks their bytes against source and records their hashes.
Edit these source files for future icon changes, never generated `web/dist`.

In Safari, open the personal deployment, then Share → Add to Home Screen.
The label defaults to **ChooChoo**. The installed shortcut keeps its icon, although
icon changes can be cached by iOS. Back up projects before replacing an existing
Home Screen installation: its storage may be separate from the Safari tab.
This adds no offline cache/service worker; opening or reloading still needs the
network. The icon does not change the browser's storage retention policy.
See [Apple's Home Screen icon guidance](https://developer.apple.com/library/archive/documentation/AppleApplications/Reference/SafariWebContent/ConfiguringWebApplications/ConfiguringWebApplications.html).
The user selected the flat icon. Physical Home Screen layout, audio interruption and Bluetooth behavior remain device checks.

## Later styling and website integration

Edit **`web/index.html`** for page CSS, framing, spacing, branding, responsive
presentation, file controls and the touch gamepad; **`web/privacy.html`** and
**`web/manual.html`** are separate pages. Do not edit `web/dist`. The canvas's
tracker screens, font rendering and layout come from shared C++/SDL source;
CSS cannot restyle their internal elements. A future change inside the canvas
requires an existing theme setting or a narrow `WEB_BUILD` conditional in shared
code, with native behavior preserved. No visual redesign is included here.

A standalone HTTPS custom **subdomain** is the simplest later integration: add it
to this Vercel project and configure DNS at that time. Asset URLs are relative and
contain no Vercel hostname. HTTPS and the default Vercel domain work independently
of the personal site's framework. No DNS or website changes are needed now.

An iframe can later point to this standalone origin, with deliberate sizing,
keyboard focus, touch scroll behavior, `allow="autoplay; fullscreen; gamepad"`
as appropriate and fullscreen permission. Browser policies still require user
interaction for audio; fullscreen support varies on iOS. Cross-origin parent
pages cannot read the tracker canvas/filesystem without an explicit messaging
interface. Third-party storage restrictions/partitioning can affect IDBFS;
changing origin (including switching to a custom domain) creates separate browser
storage. Export/import projects before moving. The deployment adds no frame-denial
headers, but Vercel protection or later CSP/frame-ancestors settings can block
embedding. This single-threaded build does not require SharedArrayBuffer or
COOP/COEP. Subpath reverse-proxy hosting and embedding remain untested; prefer a
subdomain/link first. No iframe or website integration is implemented now.

## Validation record

Startup follow-up, 2026-10-11: an autoplay-blocked browser fixture reproduced
the premature warning on the previous deployment. With the correction, initial
focus/page-show and a background/foreground cycle stay quiet, and the first
touch activates the same audio context. The standalone CSS rule adds exactly
15 pixels to the simulated safe-area layout (67 → 82 pixels from the top).
The fixture models autoplay policy and standalone CSS in Chrome, not physical iOS.

Font/Home Screen follow-up, 2026-10-11: a fresh Wasm build passed. Departure Mono
uses 320×240 CSS pixels on a simulated iPhone 15 Pro viewport; 267 measured stroke
runs were exact multiples of 3 display pixels, with none uneven. Desktop tests
likewise passed 623 stroke runs at 7 display pixels. Roboto Mono correctly falls
back to whole canvas steps. Simulated top/bottom safe insets keep the tracker
clear; page items precede the controller. Fault injection covers a frozen running
clock, a resume promise that never resolves, missing output callbacks, and failed
storage flushes. Output restarts preserve serialized song bytes; a successful
Reload app preserves the autosave and user files. These are Chrome checks, not a
physical Safari/Auto-Lock/Bluetooth certification.

Browser presentation/recovery follow-up, 2026-10-11: live color editing and
loading NostromoAmberDa2 both persisted after reload. Page colors follow the
active palette; all surrounding elements follow the canvas. Retina screenshot
inspection found zero mismatches across 302,736 source-pixel blocks at 3× scale.
Desktop 1×/1.25×/2× density, 3× phone density and resize geometry passed. Three
simulated hidden/visible cycles saved the project and recovered playable audio;
a directly suspended audio context also resumed on a canvas gesture. These tests
use Chrome; physical Safari lock/unlock is still a user check. Reproduce with
`python3 tools/test_web_presentation.py` using the same optional browser-test
requirements described below. This change affects the browser adapter/wrapper;
native renderer flags and lifecycle behavior are unchanged.

Lucky browser port, 2026-10-11: fresh Wasm compilation and Vercel function
packaging passed. Deterministic Chrome tests cover NEXT without autoplay, PLAY
with nonzero audio, LOAD without another download, four imported sample files
and project persistence after reload, cancelled downloads and retry cooldown.
A live archive request also passed NEXT, PLAY and LOAD (14 samples), without
a second download or JavaScript errors.
The native Lucky-enabled suite passed 554 cases / 112,259,078 assertions
(two opt-in checks skipped). Five relay tests cover request restrictions, size
limits, archive errors and connection failures. Physical Safari/iOS and handheld
behavior still require user testing; sampled headless audio is not a listening
test. Optional regression checks: `node --test tools/test_lucky_relay.cjs` and
`python3 tools/test_web_lucky.py` after a web build (the latter uses an existing
Playwright install, Chrome selected by `CHROME_EXECUTABLE`, and a C++ compiler
for the repository's self-authored fixture). No downloaded music is in the test.

Personal preset/appearance follow-up, 2026-10-11: fresh Wasm build and asset
validation passed; 27 focused native cases / 4,406 assertions passed. An isolated
Chromium session imported the 677-file local private library, matched every file
hash after reload, preserved existing files on repeat import, and opened USER
ZIP collections in the tracker. No upload requests occurred. Fresh appearance,
one-time legacy migration, preservation of Pad X mapping and later manual
appearance choices passed. Physical Safari/iOS import still needs user checking.

Validated locally on 2026-10-11 against the personal source at `6130710` plus
these deployment changes:

- Fresh Emscripten 6.0.10 build: passed; seven runtime files plus manifest.
  Every packed public asset matched the source byte-for-byte.
- `actionlint` 1.7.12: passed; both Vercel configurations validated against the
  official schema. Native configurations/workflows are unchanged.
- Vercel CLI 63.1.2 local static packaging and revalidation passed with dummy
  local project settings (no upload/account request). The dirty development bundle
  used validation without `--require-clean` for this packaging-only test.
- Native suite: 542 test cases, 112,249,040 assertions passed; stretch lifetime
  check passed 32 cycles with zero retained allocation bytes.
- Validator rejected missing Wasm, corrupted data, wrong SHA, stale HTML,
  unexpected files, modified JS and a dirty-source deployment.
- Headless Chrome 154: tracker/canvas initialization, native-chip project load,
  keyboard-triggered playback (89 audio callbacks; nonzero peak 0.16956),
  project save and IDBFS persistence across reload passed. Mobile-sized Chromium
  with touch enabled displayed and activated the on-screen gamepad. No JavaScript
  runtime errors. Headless audio sampling is not a speaker/listening test.
- Desktop Safari, Firefox, physical iOS Safari, real game controllers, interactive
  pattern editing, external instrument imports and background-audio recovery were
  not tested. Mobile Chromium emulation does not establish iOS compatibility.

Production was first published on 2026-10-11 at
[choochootracker-personal.vercel.app](https://choochootracker-personal.vercel.app/)
from commit `e04f9d7`, with all three Actions secrets configured externally.
The Linux-built hosted artifact passed the same headless Chrome playback,
save/reload and mobile-touch checks. `/build-info.json` is the authority for the
currently deployed commit; deployment runs link their public URL in the summary.
The initial run's immediate public check encountered a transient 404 during alias
propagation; bounded retry now covers that delay and stale prior manifests.
These deployment references belong in documentation, not application source.
Do not put credential values or project/team identifiers into this document.

References: [Vercel's GitHub Actions workflow](https://vercel.com/kb/guide/how-can-i-use-github-actions-with-vercel),
[Git deployment switches](https://vercel.com/docs/project-configuration/git-configuration),
[prebuilt deployment](https://vercel.com/docs/cli/deploy).

# ChooChooTracker Web

The existing WebAssembly application shares tracker source with native builds.
On `personal/r36h`, GitHub Actions compiles every production push and Vercel
publishes the validated artifact. `web/dist` is generated and ignored.

See [personal web deployment](../docs/personal-web-deployment.md) for account
setup, the exact build commands, previews, rollback, browser limitations and
future styling/integration. Use the repository root as the Vercel project root.

With Emscripten 6.0.10 activated:

```sh
python3 tools/web_bundle.py build
python3 -m http.server 8080 --directory web/dist
```

Open `http://localhost:8080`, then tap the canvas or press a key to activate
browser audio. Browser-only presentation lives in `web/index.html`; the tracker
inside its canvas is rendered by the shared application. Edit source files here,
not generated files in `dist`.

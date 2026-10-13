// Browser storage sequencing without a browser or third-party dependencies.
const { test } = require('node:test');
const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const vm = require('node:vm');

const html = fs.readFileSync(path.join(__dirname, '../web/index.html'), 'utf8');
const start = html.indexOf('    const syncUserStorage =');
const end = html.indexOf('    window.choochooDownloadFile =', start);
assert(start >= 0 && end > start);

function storage() {
  const pending = [];
  const context = {
    window: {}, status: { textContent: '' },
    fs: { syncfs(populate, done) { assert.equal(populate, false); pending.push(done); } },
  };
  vm.createContext(context);
  vm.runInContext(`
    let storageSyncing = false, storagePending = false, storageCallbacks = [];
    const getFs = () => fs;
    ${html.slice(start, end)}
    this.sync = syncUserStorage;
  `, context);
  return { context, pending };
}

test('a save arriving during a write gets another flush before its callback', () => {
  const { context: c, pending } = storage();
  const completed = [];
  c.sync(error => { assert.equal(error, null); completed.push('first'); });
  c.sync(error => { assert.equal(error, null); completed.push('second'); });
  c.sync(error => { assert.equal(error, null); completed.push('third'); });
  assert.equal(pending.length, 1, 'no concurrent IDBFS writes');
  pending.shift()(null);
  assert.deepEqual(completed, ['first']);
  assert.equal(pending.length, 1, 'queued work was not dropped');
  pending.shift()(null);
  assert.deepEqual(completed, ['first', 'second', 'third']);
  assert.equal(pending.length, 0);
});

test('settings notifications use the same queue as a waiting reload', () => {
  const { context: c, pending } = storage();
  let reloadReady = false;
  c.window.choochooSettingsSaved();
  c.window.choochooSettingsSaved();
  c.sync(error => { assert.equal(error, null); reloadReady = true; });
  pending.shift()(null);
  assert.equal(reloadReady, false);
  pending.shift()(null);
  assert.equal(reloadReady, true);
});

test('storage errors reach callbacks and do not strand queued work', () => {
  const { context: c, pending } = storage();
  const error = new Error('storage unavailable');
  const results = [];
  c.sync(e => results.push(e));
  c.sync(e => results.push(e));
  pending.shift()(error);
  assert.equal(results[0], error);
  assert.match(c.status.textContent, /Could not save browser storage/);
  pending.shift()(null);
  assert.deepEqual(results, [error, null]);
});

test('missing filesystem reports an error instead of silently losing completion', () => {
  const { context: c, pending } = storage();
  c.fs = null;
  let error;
  c.sync(e => { error = e; });
  assert.match(error.message, /not ready/);
  assert.equal(pending.length, 0);
});

function demoStorage(t) {
  const base = fs.mkdtempSync(path.join(require('node:os').tmpdir(), 'choo-demo-cleanup-'));
  t.after(() => fs.rmSync(base, { recursive: true, force: true }));
  const local = name => path.join(base, name);
  const adapter = {
    analyzePath: name => ({ exists: fs.existsSync(local(name)) }),
    stat: name => fs.statSync(local(name)),
    isFile: mode => (mode & 0o170000) === 0o100000,
    isDir: mode => (mode & 0o170000) === 0o040000,
    readFile: (name, options) => fs.readFileSync(local(name), options),
    writeFile(name, bytes) {
      fs.mkdirSync(path.dirname(local(name)), { recursive: true });
      fs.writeFileSync(local(name), bytes);
    },
    readdir: name => ['.', '..', ...fs.readdirSync(local(name))],
    unlink: name => fs.unlinkSync(local(name)),
    rmdir: name => fs.rmdirSync(local(name)),
  };
  const context = { crypto: require('node:crypto').webcrypto };
  vm.createContext(context);
  const begin = html.indexOf('    const seededRoughDemoHashes =');
  vm.runInContext(html.slice(begin, start) + '\nthis.cleanup = removeSeededRoughDemos;', context);
  const collection = path.join(__dirname, '../tracker/packaging/web/roughdemos');
  const records = JSON.parse(fs.readFileSync(path.join(collection, 'collection.json'))).projects;
  return { adapter, context, records, collection };
}

test('removes exact seeded demos from both old roots, preserving edits and other user data', async t => {
  const { adapter: f, context: c, records, collection } = demoStorage(t);
  const roots = ['/user/projects/roughdemos', '/user/projects/rough demos'];
  for (const root of roots) {
    for (const record of records) f.writeFile(root + '/' + record.file, fs.readFileSync(path.join(collection, record.file)));
  }
  const edited = roots[1] + '/' + records[0].file;
  f.writeFile(edited, 'edited song');
  const extra = roots[1] + '/my song.cct';
  f.writeFile(extra, 'new song');
  for (const name of ['/user/autosave.cct', '/user/projects/mine.cct', '/user/instruments/USER/sid/mine.zip']) f.writeFile(name, 'keep');
  f.writeFile('/user/settings.txt', 'projectPath: ' + roots[0] + '/Blue_Hour\notherSetting: 42\n');
  await c.cleanup(f);
  assert.equal(f.analyzePath(roots[0]).exists, false);
  for (const record of records) {
    const name = roots[1] + '/' + record.file;
    assert.equal(f.analyzePath(name).exists, name === edited, name);
  }
  assert.equal(f.readFile(edited, 'utf8'), 'edited song');
  assert.equal(f.readFile(extra, 'utf8'), 'new song');
  for (const name of ['/user/autosave.cct', '/user/projects/mine.cct', '/user/instruments/USER/sid/mine.zip']) assert.equal(f.readFile(name, 'utf8'), 'keep');
  assert.equal(f.readFile('/user/settings.txt', 'utf8'), 'projectPath: /projects\notherSetting: 42\n');
  assert.equal(f.readFile('/user/.rough-demo-cleanup-v1', 'utf8'), '1\n');
  // Once complete, deliberate later saves must not be removed again.
  const later = roots[0] + '/' + records[0].file;
  f.writeFile(later, fs.readFileSync(path.join(collection, records[0].file)));
  await c.cleanup(f);
  assert.equal(f.analyzePath(later).exists, true);
});

test('keeps a selected demo directory with edits, and retries a failed hash check', async t => {
  const { adapter: f, context: c, records, collection } = demoStorage(t);
  const root = '/user/projects/rough demos';
  const demo = root + '/' + records[0].file;
  f.writeFile(demo, fs.readFileSync(path.join(collection, records[0].file)));
  f.writeFile(root + '/my song.cct', 'edited song');
  const settings = 'projectPath: ' + root + '\n';
  f.writeFile('/user/settings.txt', settings);
  const crypto = c.crypto;
  c.crypto = { subtle: { digest() { throw new Error('hash failed'); } } };
  await assert.rejects(c.cleanup(f), /hash failed/);
  assert.equal(f.analyzePath('/user/.rough-demo-cleanup-v1').exists, false);
  assert.equal(f.analyzePath(demo).exists, true);
  c.crypto = crypto;
  await c.cleanup(f);
  assert.equal(f.analyzePath(demo).exists, false);
  assert.equal(f.readFile('/user/settings.txt', 'utf8'), settings);
});

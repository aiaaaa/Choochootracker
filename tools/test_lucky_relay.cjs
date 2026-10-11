const { test, after } = require('node:test');
const assert = require('node:assert/strict');
const relay = require('../api/lucky.js');
const originalFetch = global.fetch;
after(() => { global.fetch = originalFetch; });
function response() {
  return { headers: {}, code: 200, setHeader(k, v) { this.headers[k] = v; },
    status(code) { this.code = code; return this; }, send(body) { this.body = body; return this; } };
}
async function request(method = 'GET', url = '/api/lucky') {
  const result = response(); await relay({ method, url }, result); return result;
}
test('only GET with no parameters can contact the fixed archive page', async () => {
  global.fetch = async () => { throw new Error('Must not fetch'); };
  assert.equal((await request('POST')).code, 405);
  assert.equal((await request('GET', '/api/lucky?url=https://example.com')).code, 400);
});
test('successful relay returns plain text without forwarding cookies or client headers', async () => {
  global.fetch = async (url, options) => {
    assert.equal(url, 'https://modarchive.org/index.php?request=view_player&query=random');
    assert.equal(options.redirect, 'manual');
    assert.deepEqual(Object.keys(options.headers), ['User-Agent']);
    return new Response('<html>random player</html>', { headers: { 'Set-Cookie': 'private=test' } });
  };
  const result = await request();
  assert.equal(result.code, 200);
  assert.equal(result.body.toString(), '<html>random player</html>');
  assert.equal(result.headers['Cache-Control'], 'no-store');
  assert.equal(result.headers['Content-Type'], 'text/plain; charset=utf-8');
  assert.equal(result.headers['Set-Cookie'], undefined);
});
test('archive denials and rate limits are respected; redirects are not followed', async () => {
  for (const [upstream, expected] of [[403,403], [404,404], [429,429], [503,503], [302,502]]) {
    global.fetch = async () => new Response('', { status: upstream, headers: { 'Retry-After': '60' } });
    const result = await request();
    assert.equal(result.code, expected);
    assert.equal(result.headers['Retry-After'], '60');
  }
});
test('oversized declared and streamed pages are rejected', async () => {
  for (const headers of [{ 'Content-Length': '262145' }, {}]) {
    global.fetch = async () => new Response(new Uint8Array(262145), { headers });
    assert.equal((await request()).code, 502);
  }
});
test('transport failures return a bounded error without internal details', async () => {
  global.fetch = async () => { throw new Error('internal connection detail'); };
  const result = await request(); assert.equal(result.code, 502);
  assert.equal(result.body, 'Archive connection failed.');
});

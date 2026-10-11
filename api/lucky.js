// Fixed-purpose relay for the archive's random-player page (which lacks CORS).
// Module bytes go directly to the browser; no user files pass through this API.
const RANDOM_PAGE = 'https://modarchive.org/index.php?request=view_player&query=random';
const MAX_BYTES = 256 * 1024;

module.exports = async function lucky(req, res) {
  res.setHeader('Cache-Control', 'no-store');
  res.setHeader('Content-Type', 'text/plain; charset=utf-8');
  res.setHeader('X-Content-Type-Options', 'nosniff');
  if (req.method !== 'GET') {
    res.setHeader('Allow', 'GET');
    return res.status(405).send('Use GET.');
  }
  if (new URL(req.url, 'https://local.invalid').search) return res.status(400).send('No parameters accepted.');
  const controller = new AbortController();
  const timer = setTimeout(() => controller.abort(), 20000);
  try {
    // No cookies, client headers, arbitrary URLs, or redirects are forwarded.
    const upstream = await fetch(RANDOM_PAGE, {
      redirect: 'manual', signal: controller.signal,
      headers: { 'User-Agent': 'ChooChooTracker-ModLucky/0.1 (personal web experiment; aiaaaa)' },
    });
    const retry = upstream.headers.get('retry-after');
    if (retry && retry.length <= 128) res.setHeader('Retry-After', retry);
    if (upstream.status !== 200) {
      await upstream.body?.cancel();
      const status = [403, 404, 429, 503].includes(upstream.status) ? upstream.status : 502;
      return res.status(status).send('Archive request failed.');
    }
    if (Number(upstream.headers.get('content-length')) > MAX_BYTES) {
      controller.abort();
      return res.status(502).send('Archive page exceeds size limit.');
    }
    const chunks = [];
    let length = 0;
    for await (const chunk of upstream.body) {
      length += chunk.length;
      if (length > MAX_BYTES) { controller.abort(); return res.status(502).send('Archive page exceeds size limit.'); }
      chunks.push(chunk);
    }
    return res.status(200).send(Buffer.concat(chunks));
  } catch (_) {
    return res.status(controller.signal.aborted ? 504 : 502).send('Archive connection failed.');
  } finally { clearTimeout(timer); }
};

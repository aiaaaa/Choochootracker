"""Validate our conservative Vita LiveArea PNG profile without image libraries."""
import struct
import zlib
import xml.etree.ElementTree as ET

IMAGES = {
    'sce_sys/icon0.png': (128, 128),
    'sce_sys/livearea/contents/bg.png': (840, 500),
    'sce_sys/livearea/contents/startup.png': (280, 158),
}

def validate_png(data, name, dimensions):
    def reject(reason):
        raise RuntimeError(f'LiveArea {name}: {reason}; use an opaque, non-interlaced 8-bit indexed PNG')
    if len(data) < 33 or data[:8] != b'\x89PNG\r\n\x1a\n': reject('invalid PNG signature/header')
    if data[12:16] != b'IHDR' or struct.unpack('>I', data[8:12])[0] != 13: reject('invalid IHDR')
    width, height, depth, color, compression, filtering, interlace = struct.unpack('>IIBBBBB', data[16:29])
    if (width, height) != dimensions: reject('incorrect dimensions')
    # All our artwork is opaque. Indexed RGB is the tested packaging contract;
    # permitting other PNG types here previously missed installer 0x8010113D.
    if (depth, color, compression, filtering, interlace) != (8, 3, 0, 0, 0): reject('unsupported PNG encoding')
    offset, palette, ended, compressed = 8, False, False, bytearray()
    while offset + 12 <= len(data):
        size = struct.unpack('>I', data[offset:offset+4])[0]
        end = offset + 12 + size
        if end > len(data): reject('truncated chunk')
        kind = data[offset+4:offset+8]
        payload = data[offset+8:end-4]
        if zlib.crc32(kind + payload) & 0xffffffff != struct.unpack('>I', data[end-4:end])[0]: reject('chunk checksum mismatch')
        if kind == b'PLTE':
            if not 3 <= size <= 768 or size % 3: reject('invalid palette')
            palette = True
        elif kind == b'tRNS' and any(alpha != 255 for alpha in payload): reject('unexpected transparency')
        elif kind == b'IDAT':
            if not palette: reject('missing palette before pixels')
            compressed.extend(payload)
        elif kind == b'IEND':
            if size or end != len(data): reject('invalid PNG end')
            ended = True
            break
        offset = end
    if not palette or not compressed or not ended: reject('incomplete PNG')
    expected = (width + 1) * height
    decoder = zlib.decompressobj()
    try:
        raw = decoder.decompress(compressed, expected + 1)
    except zlib.error:
        reject('invalid compressed pixels')
    if len(raw) != expected or not decoder.eof or decoder.unused_data: reject('incorrect pixel data length')
    if any(raw[row * (width + 1)] > 4 for row in range(height)): reject('invalid row filter')

def validate_livearea(read):
    for name, dimensions in IMAGES.items():
        validate_png(read(name), name, dimensions)
    try:
        root = ET.fromstring(read('sce_sys/livearea/contents/template.xml'))
        if root.tag != 'livearea' or root.get('style') != 'a1': raise ValueError('unsupported layout')
        if root.findtext('livearea-background/image') != 'bg.png': raise ValueError('background reference')
        if root.findtext('gate/startup-image') != 'startup.png': raise ValueError('startup reference')
    except (ET.ParseError, ValueError) as error:
        raise RuntimeError('Invalid LiveArea template: ' + str(error)) from error

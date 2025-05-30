/** Copyright (c) 2025, Wu Jin <wujin.developer@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "xcomm-ringbuffer.h"

static inline uint32_t _ringbuffer_rounddown_pow_of_two(uint32_t n) {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return (n + 1) >> 1;
}

static inline void _ringbuffer_internal_write(
    xcomm_ringbuf_t* ring, const void* src, uint32_t len, uint32_t off) {
    uint32_t size  = ring->mask + 1;
    uint32_t esize = ring->esz;
    uint32_t l;

    off &= ring->mask;
    if (esize != 1) {
        off  *= esize;
        size *= esize;
        len  *= esize;
    }
    l = (len) < (size - off) ? (len) : (size - off);

    memcpy(ring->buf + off, src, l);
    memcpy(ring->buf, (uint8_t*)src + l, len - l);
}

static inline void _ringbuffer_internal_read(
    xcomm_ringbuf_t* ring, void* dst, uint32_t len, uint32_t off) {
    uint32_t size = ring->mask + 1;
    uint32_t esize = ring->esz;
    uint32_t l;

    off &= ring->mask;
    if (esize != 1) {
        off *= esize;
        size *= esize;
        len *= esize;
    }
    l = (len) < (size - off) ? (len) : (size - off);

    memcpy(dst, ring->buf + off, l);
    memcpy((uint8_t*)dst + l, ring->buf, len - l);
}

static inline uint32_t
_ringbuffer_internal_read_peek(xcomm_ringbuf_t* ring, void* buf, uint32_t len) {
    uint32_t l;
    l = ring->wpos - ring->rpos;
    if (len > l) {
        len = l;
    }
    _ringbuffer_internal_read(ring, buf, len, ring->rpos);
    return len;
}

void xcomm_ringbuf_init(
    xcomm_ringbuf_t* ring, uint32_t esize, uint32_t bufsize) {
    ring->buf = malloc(bufsize);
    ring->esz = esize;
    ring->mask = _ringbuffer_rounddown_pow_of_two(bufsize / esize) - 1;
    ring->wpos = 0;
    ring->rpos = 0;
}

void xcomm_ringbuf_destroy(xcomm_ringbuf_t* ring) {
    if (ring->buf) {
        free(ring->buf);
        ring->buf = NULL;
    }
}

bool xcomm_ringbuf_full(xcomm_ringbuf_t* ring) {
    return xcomm_ringbuf_len(ring) > ring->mask;
}

bool xcomm_ringbuf_empty(xcomm_ringbuf_t* ring) {
    return ring->wpos == ring->rpos;
}

uint32_t xcomm_ringbuf_len(xcomm_ringbuf_t* ring) {
    return ring->wpos - ring->rpos;
}

uint32_t xcomm_ringbuf_cap(xcomm_ringbuf_t* ring) {
    return ring->mask + 1;
}

uint32_t xcomm_ringbuf_avail(xcomm_ringbuf_t* ring) {
    return xcomm_ringbuf_cap(ring) - xcomm_ringbuf_len(ring);
}

uint32_t xcomm_ringbuf_write(
    xcomm_ringbuf_t* ring, const void* buf, uint32_t entry_count) {
    uint32_t avail = xcomm_ringbuf_avail(ring);
    if (entry_count > avail) {
        entry_count = avail;
    }
    _ringbuffer_internal_write(ring, buf, entry_count, ring->wpos);

    ring->wpos += entry_count;
    return entry_count;
}

uint32_t
xcomm_ringbuf_read(xcomm_ringbuf_t* ring, void* buf, uint32_t entry_count) {
    entry_count = _ringbuffer_internal_read_peek(ring, buf, entry_count);
    ring->rpos += entry_count;

    return entry_count;
}
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

_Pragma("once")

#include <stdbool.h>

#define xcomm_logd(...)    xcomm_logger_log(XCOMM_LOGGER_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define xcomm_logi(...)    xcomm_logger_log(XCOMM_LOGGER_LEVEL_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define xcomm_logw(...)    xcomm_logger_log(XCOMM_LOGGER_LEVEL_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define xcomm_loge(...)    xcomm_logger_log(XCOMM_LOGGER_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

typedef enum xcomm_logger_level_e xcomm_logger_level_t;

enum xcomm_logger_level_e {
    XCOMM_LOGGER_LEVEL_DEBUG,
    XCOMM_LOGGER_LEVEL_INFO,
    XCOMM_LOGGER_LEVEL_WARN,
    XCOMM_LOGGER_LEVEL_ERROR,
};

extern void xcomm_logger_init(const char* restrict filename, xcomm_logger_level_t level, bool async);
extern void xcomm_logger_destroy(void);
extern void xcomm_logger_set_callback(void (*callback)(xcomm_logger_level_t level, const char* restrict msg));

/* used by internal */
extern void xcomm_logger_log(xcomm_logger_level_t level, const char* restrict file, int line, const char* restrict fmt, ...);

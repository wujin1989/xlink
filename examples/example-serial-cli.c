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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xcomm.h"

int main(void) {
    xcomm_serial_config_t serial_config = {
        .device   = "COM2",
        .baudrate = XCOMM_SERIAL_BAUDRATE_9600,
        .parity   = XCOMM_SERIAL_PARITY_NO,
        .databits = XCOMM_SERIAL_DATABITS_CS8,
        .stopbits = XCOMM_SERIAL_STOPBITS_ONE,
        .timeout_ms  = 0,
    };
    xcomm_serial_t* serial = xcomm_serial.dial(&serial_config);
    if (!serial) {
        return -1;
    }
    char* buffer = "hello world";
    xcomm_serial.send(serial, buffer, strlen(buffer));
    printf("write %s to serial.\n", buffer);

    xcomm_serial.close(serial);
    return 0;
}
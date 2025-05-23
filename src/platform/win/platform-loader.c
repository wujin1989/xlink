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

#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include "platform/platform-loader.h"

void* platform_loader_create(char* restrict file) {
    for (int i = 0; i < strlen(file); i++) {
        if (file[i] == '/') {
            file[i] = '\\';
        }
    }
    int wlen = MultiByteToWideChar(GetACP(), 0, file, -1, NULL, 0);
    if (wlen <= 0) {
        return NULL;
    }
    wchar_t* wfile = (wchar_t*)malloc(wlen * sizeof(wchar_t));
    if (!wfile) {
        return NULL;
    }
    MultiByteToWideChar(GetACP(), 0, file, -1, wfile, wlen);

    HMODULE hModule = LoadLibraryW(wfile);
    free(wfile);
    return (void*)hModule;
}

void* platform_loader_load(void* module, const char* restrict func) {
    void* ret = (void*)GetProcAddress((HMODULE)module, func);
    if (ret == NULL) {
        platform_loader_destroy(module);
    }
    return ret;
}

void platform_loader_destroy(void* module) {
    FreeLibrary((HMODULE)module);
}
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

#include "xcomm-list.h"

#define xcomm_stack_data(x, t, m) ((t *)((char *)(x) - offsetof(t, m)))

typedef struct xcomm_list_node_s xcomm_stack_t;
typedef struct xcomm_list_node_s xcomm_stack_node_t;

extern void xcomm_stack_init(xcomm_stack_t* stack);
extern void xcomm_stack_push(xcomm_stack_t* stack, xcomm_stack_node_t* node);
extern bool xcomm_stack_empty(xcomm_stack_t* stack);
extern void xcomm_stack_swap(xcomm_stack_t* stack1, xcomm_stack_t* stack2);
extern xcomm_stack_node_t* xcomm_stack_pop(xcomm_stack_t* stack);
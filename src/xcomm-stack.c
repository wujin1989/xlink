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

#include "xcomm-list.h"
#include "xcomm-stack.h"

void xcomm_stack_init(xcomm_stack_t* stack) {
    xcomm_list_init(stack);
}

void xcomm_stack_push(xcomm_stack_t* stack, xcomm_stack_node_t* node) {
    xcomm_list_insert_tail(stack, node);
}

bool xcomm_stack_empty(xcomm_stack_t* stack) {
    return xcomm_list_empty(stack);
}

xcomm_stack_node_t* xcomm_stack_pop(xcomm_stack_t* stack) {
    if (xcomm_stack_empty(stack)) {
        return NULL;
    }
    xcomm_stack_node_t* node = xcomm_list_tail(stack);
    xcomm_list_remove(node);
    return node;
}

void xcomm_stack_swap(xcomm_stack_t* stack1, xcomm_stack_t* stack2) {
    xcomm_list_swap(stack1, stack2);
}
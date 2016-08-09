/*
 * Copyright (c) 2015 Kaprica Security, Inc.
 *
 * Permission is hereby granted, cgc_free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef __LIST__H
#define __LIST__H
#include "common.h"

// Linked cgc_list w/ void * data
typedef struct cgc_list cgc_list;
struct cgc_list {
  cgc_list *n;
  cgc_list *p;
  const DATA d;
};

cgc_list *cgc_init_list(const DATA d);
void cgc_append_list(cgc_list **l, const DATA d, int dups);
cgc_size_t cgc_len_list(const cgc_list *l);
cgc_list *cgc_copy_list(const cgc_list *l, cgc_size_t start, cgc_size_t stop);
const DATA cgc_lindex(const cgc_list *l, cgc_ssize_t i);
const void *cgc_random_element(cgc_list *l);
void cgc_free_list(const cgc_list *l, int ld);
void cgc_free_list_of_lists(const cgc_list *l, int ld);
#endif /* __LIST__H */

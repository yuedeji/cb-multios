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
#include <stdlib.h>
#include <string.h>
#include "dict.h"

#define INITIAL_SIZE 8

static unsigned int cgc_do_hash(cgc_dict_t *dict, const char *name)
{
    unsigned int hash = 0;
    char c;

    while ((c = *name++) != 0)
    {
        hash = hash * 1337 + c;
    }

    return hash % dict->length;
}

static void cgc_do_insert(cgc_dict_t *dict, cgc__record_t *r)
{
    cgc__record_t **head = &dict->tbl[cgc_do_hash(dict, r->name)];

    r->tbl_prev = NULL;
    r->tbl_next = *head;
    if (r->tbl_next != NULL)
        r->tbl_next->tbl_prev = r;
    *head = r;

    r->prev = NULL;
    r->next = dict->head;
    if (r->next != NULL)
        r->next->prev = r;
    dict->head = r;
}

static void cgc_do_unlink(cgc_dict_t *dict, cgc__record_t *r)
{
    cgc__record_t **head = &dict->tbl[cgc_do_hash(dict, r->name)];
    
    if (*head == r)
        *head = r->tbl_next;
    if (r->tbl_prev != NULL)
        r->tbl_prev->tbl_next = r->tbl_next;
    if (r->tbl_next != NULL)
        r->tbl_next->tbl_prev = r->tbl_prev;
    
    if (dict->head == r)
        dict->head = r->next;
    if (r->prev != NULL)
        r->prev->next = r->next;
    if (r->next != NULL)
        r->next->prev = r->prev;
}

static int cgc_enlarge_tbl(cgc_dict_t *dict, unsigned int new_size)
{
    cgc__record_t **new_tbl, *r, *n;

    if (dict->length >= new_size)
        return 1;

    new_tbl = cgc_realloc(dict->tbl, new_size * sizeof(cgc__record_t *));
    if (new_tbl == NULL)
        return 0;

    cgc_memset(new_tbl, 0, new_size * sizeof(cgc__record_t *));
    dict->tbl = new_tbl;
    dict->length = new_size;

    // reinsert items
    r = dict->head;
    dict->head = NULL;
    for (; r != NULL; r = n)
    {
        n = r->next;
        r->next = r->prev = NULL;

        cgc_do_insert(dict, r);
    }

    return 1;
}

int cgc_dict_init(cgc_dict_t *dict, cgc_record_free_t free_fn)
{
    dict->free_fn = free_fn;
    dict->head = NULL;
    dict->tbl = NULL;
    dict->count = 0;
    dict->length = 0;

    if (!cgc_enlarge_tbl(dict, INITIAL_SIZE))
        return 0;

    return 1;
}

void cgc_dict_free(cgc_dict_t *dict)
{
    cgc__record_t *r, *n;
    for (r = dict->head; r != NULL; r = n)
    {
        n = r->next;

        dict->free_fn(r);
        cgc_free(r);
    }
}

void *cgc_dict_get(cgc_dict_t *dict, const char *name)
{
    cgc__record_t *r = dict->tbl[cgc_do_hash(dict, name)];

    for (; r != NULL; r = r->next)
        if (cgc_strcmp(r->name, name) == 0)
            return r;

    return NULL;
}

int cgc_dict_add(cgc_dict_t *dict, const char *name, void *value)
{
    cgc__record_t *r = value, *old;
    if ((old = cgc_dict_get(dict, name)) == NULL)
    {
        if ((dict->count + 1) >= dict->length * 0.7)
            if (!cgc_enlarge_tbl(dict, dict->length * 2))
                return 0;
        r->name = cgc_strdup(name);
        if (r->name == NULL)
            return 0;
        cgc_do_insert(dict, r);
        dict->count++;
    }
    else
    {
        r->name = old->name;

        cgc_do_unlink(dict, old);
        dict->free_fn(old);
        cgc_free(old);

        cgc_do_insert(dict, r);
    }

    return 1;
}

/*

Author: Debbie Nuttall <debbie@cromulence.com>

Copyright (c) 2016 Cromulence LLC

Permission is hereby granted, cgc_free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <libcgc.h>
#include "malloc.h"
#include "stdio.h"
#include "linkedlist.h"


cgc_linkedList *cgc_NewList(int type) {
  cgc_linkedList *list = cgc_calloc(sizeof(cgc_linkedList));
  list->type = type;
  return list;
}


int cgc_AddToList(cgc_linkedList *list, void *object, int type) {
  if (list == NULL) {
    return -1;
  }
  if (object == NULL) {
    return -1;
  }
  if (type != list->type) {
    return -1;
  }
  cgc_link *newLink = cgc_calloc(sizeof(cgc_link));
  newLink->type = type;
  newLink->object = object;
  if (list->root == NULL) {
    list->root = newLink;
    return 1;
  }
  cgc_link *next = list->root;
  while (next->next != NULL) {
    next = next->next;
  }
  next->next = newLink;
  newLink->prev = next;
  return 1;
}


int cgc_RemoveFromList(cgc_linkedList *list, void *object) 
{
  if (list == NULL) 
  {
    return -1;
  }
  if (object == NULL) 
  {
    return -1;
  }
  if (list->root->object == object) 
  {
    cgc_link *toDelete = list->root ;
    list->root = list->root->next;
    list->root->prev = NULL;
    cgc_free(toDelete);
    return 1;
  }
  cgc_link *prev= list->root;
  cgc_link *this = list->root->next;
  while (this != NULL && this->object != object) 
  {
    prev = this;
    this = this->next;
  }
  if (this != NULL) 
  {
    cgc_link *toDelete = this;
    prev->next = this->next;
    if (prev->next != NULL) 
    {
      prev->next->prev = prev;
    }
    return 1;
  } 
  return -1;
}

void cgc_DumpList(cgc_linkedList *list) {
  if (list == NULL)
  {
    cgc_printf("Empty List\n");
    return;
  }
  cgc_printf("List Type: $x\n", list->type);
  int i = 0;
  cgc_link *this = list->root;
  while (this != NULL) {
    cgc_printf("$d) $s\n", i++, (char *)this->object);
    this = this->next;
  }
}
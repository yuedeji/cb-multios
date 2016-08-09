/*

Author: John Berry <hj@cromulence.co>

Copyright (c) 2014 Cromulence LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
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

#ifndef __CITYPARSERS_H__
#define __CITYPARSERS_H__

#include <libcgc.h>
#include <stdlib.h>
#include "string.h"
#include "service.h"
#include "helper.h"
#include "genericParsers.h"

int cgc_cityMenu( cgc_pCity ci);
void cgc_initCity( cgc_pCity ci );
void cgc_freeCity( cgc_pCity ci );
void cgc_printCityInfo( cgc_pCity ci );
cgc_pCity cgc_cityTopLevel( cgc_pstring str);
char *cgc_extractUrl( cgc_pstring str );
char *cgc_extractMayor( cgc_pstring str );

#endif


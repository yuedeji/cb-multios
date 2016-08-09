/*

Copyright (c) 2016 Cromulence LLC

Authors: Bryce Kerley <bk@cromulence.com>

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

#pragma once

#define UINT8_MAX 0xff
typedef unsigned char cgc_uint8;
typedef   signed char cgc_sint8;

#define UINT16_MAX 0xffff
typedef unsigned short int cgc_uint16;
typedef   signed short int cgc_sint16;

#define UINT32_MAX 0xffffffff
typedef unsigned int cgc_uint32;
typedef   signed int cgc_sint32;

#define UINT64_MAX 0xffffffffffffffff
typedef unsigned long long cgc_uint64;
typedef   signed long long cgc_sint64;

typedef float  cgc_float32;
typedef double cgc_float64;

void cgc_types_check();

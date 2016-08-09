/*
 * Copyright (c) 2016 Kaprica Security, Inc.
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
#pragma once

#include "node.h"

class cgc_CmlNode : public cgc_Node
{
    friend class cgc_Node;
private:
    cgc_CmlNode(const cgc_String *cgc_ns, const cgc_String *cgc_tag);
public:
    virtual void cgc_set_attr(const char *cgc_name, cgc_String *value);

    inline const cgc_String *cgc_childns() { return d_childns; }
    inline const cgc_String *cgc_spec() { return d_spec; }
    inline const cgc_String *cgc_version() { return d_version; }
    inline unsigned long long cgc_version_code() { return d_version_code; }
private:
    const cgc_String *d_spec;
    const cgc_String *d_version;
    unsigned long long d_version_code;
    const cgc_String *d_childns;
};

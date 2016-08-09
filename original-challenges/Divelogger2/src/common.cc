/*

Author: Jason Williams <jdw@cromulence.com>

Copyright (c) 2014 Cromulence LLC

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
#include "common.h"

#define LINE_BUF_MAX        (512)

void cgc_ReadLine( cgc_String &sLine )
{
    char lineBuffer[LINE_BUF_MAX+1];

    cgc_getline( lineBuffer, LINE_BUF_MAX );

    sLine.cgc_SetString( lineBuffer );
}

cgc_size_t cgc_recv_bytes( cgc_uint8_t *pDest, cgc_size_t count )
{
    cgc_size_t pos = 0;
    cgc_size_t rx_count = 0;

    while ( pos < count )
    {
        if ( receive( STDIN, (pDest+pos), 1, &rx_count ) != 0 )
            goto end;

        if ( rx_count != 1 )
            goto end;

        pos++;
    }

end:
    return (pos);
}

void cgc_ReadUint32( cgc_uint32_t &value )
{
    int retvalue;
    cgc_size_t rx_count = 1;

    if ( cgc_recv_bytes( (cgc_uint8_t*)&value, sizeof(value) ) != sizeof(value) )
        _terminate( -1 );
}

cgc_uint32_t cgc_StringToInt( const cgc_String& sInStr )
{
    return cgc_atoi( sInStr.cgc_StripSpaces().cgc_c_str() );
}

/*

Author: Jason Williams <jdw@cromulence.com>

Copyright (c) 2015 Cromulence LLC

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
extern "C"
{
#include <libcgc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <prng.h>
}

#include "comms.h"

cgc_uint32_t cgc_RecvData( cgc_int32_t fd, cgc_uint8_t *pDest, cgc_uint32_t recvLen )
{
	if ( pDest == NULL )
		return (0);

	if ( recvLen == 0 )
		return (0);

	cgc_uint32_t curRecvLen = 0;

	while ( curRecvLen < recvLen )
	{
		cgc_size_t num_bytes;
		cgc_uint32_t recvRemaining = (recvLen - curRecvLen);

		if ( receive( fd, (pDest+curRecvLen), recvRemaining, &num_bytes ) != 0 )
			_terminate( -1 );

		if ( num_bytes == 0 )
			_terminate( -1 );

		curRecvLen += num_bytes;			 
	}

	return curRecvLen;
}

cgc_uint32_t cgc_SendData( cgc_int32_t fd, cgc_uint8_t *pSource, cgc_uint32_t sendLen )
{
	if ( pSource == NULL )
		return (0);

	if ( sendLen == 0 )
		return (0);

	cgc_uint32_t curSendLen = 0;

	while ( curSendLen < sendLen )
	{
		cgc_size_t num_bytes;
		cgc_uint32_t sendRemaining = (sendLen - curSendLen);

		if ( transmit( fd, (pSource+curSendLen), sendRemaining, &num_bytes ) != 0 )
			_terminate( -1 );

		if ( num_bytes == 0 )
			_terminate( -1 );

		curSendLen += num_bytes;
	}

	return (curSendLen);
}

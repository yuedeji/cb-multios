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
#include <libcgc.h>
#include <stdlib.h>
#include <stdint.h>

int cgc_isspace( int c )
{
    if ( c == ' ' ||
         c == '\t' ||
         c == '\n' ||
         c == '\v' ||
         c == '\f' ||
         c == '\r' )
        return 1;
    else
        return 0;
}

int cgc_isdigit( int c )
{
    if ( c >= '0' && c <= '9' )
        return 1;
    else
        return 0;
}

int cgc_isnan( double val )
{
    return __builtin_isnan( val );
}

int cgc_isinf( double val )
{
    return __builtin_isinf( val );
}

double cgc_atof(const char* str)
{
    if ( str == NULL )
        return 0.0;

    double val = 0.0;
    double scale = 0.1;
    int sign = 1;
    int part;

    // Skip whitespace
    while ( cgc_isspace( str[0] ) )
        str++;

    part = 0; // First part (+/-/./number is acceptable)

    while( str[0] != '\0' )
    {
        if ( str[0] == '-' )
        {
            if ( part != 0 )
                return 0.0;

            sign = -1;
            part++;
        }
        else if ( str[0] == '+' )
        {
            if ( part != 0 )
                return 0.0;

            part++;
        }
        else if ( str[0] == '.' )
        {
            if ( part == 0 || part == 1 )
                part = 2;
            else
                return 0.0;
        }
        else if ( cgc_isdigit( *str ) )
        {
            if ( part == 0 || part == 1 )
            {
                // In integer part
                part = 1;
                val = (val * 10.0) + (*str - '0');
            }
            else if ( part == 2 )
            {
                val += ((*str - '0') * scale);
                scale /= 10.0;
            }
            else
            {
                // part invalid
                return 0.0;
            }
        }
        else
            break;

        str++;
    }

    return (sign * val);
}


int cgc_atoi(const char* str)
{
    if ( str == NULL )
        return 0;

    int integer_part = 0;
    int sign = 1;
    int part;
    int digit_count = 0;

    // Skip whitespace
    while ( cgc_isspace( str[0] ) )
        str++;

    part = 0; // First part (+/-/number is acceptable)

    while( str[0] != '\0' )
    {
        if ( str[0] == '-' )
        {
            if ( part != 0 )
                return 0;

            sign = -1;
            part++;
        }
        else if ( str[0] == '+' )
        {
            if ( part != 0 )
                return 0;

            part++;
        }
        else if ( cgc_isdigit( *str ) )
        {
            if ( part == 0 || part == 1 )
            {
                // In integer part
                part = 1;
                integer_part = (integer_part * 10) + (*str - '0');

                digit_count++;

                if ( digit_count == 9 )
                    break;
            }
            else
            {
                // part invalid
                return 0;
            }
        }
        else
            break;

        str++;
    }

    return (sign * integer_part);
}

char *cgc_strcpy( char *dest, char *src )
{
    cgc_size_t i;

    for ( i = 0; ; i++ )
    {
        if ( src[i] == '\0' )
            break;

        dest[i] = src[i];
    }
    dest[i] = '\0';

    return (dest);
}

void cgc_bzero( void *buff, cgc_size_t len )
{
    cgc_size_t index = 0;
    unsigned char *c = buff;

    if ( buff == NULL ) {
        goto end;
    }

    if ( len == 0 ) {
        goto end;
    }

    for ( index = 0; index < len; index++ ) {
        c[index] = 0x00;
    }

end:
    return;
}

int cgc_strcmp( const char *s1, const char *s2 ) 
{
    while ( *s1 && (*s1 == *s2) ) 
    {
      s1++,s2++;
    }
    return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

char *cgc_strncat ( char *dest, const char *src, cgc_size_t n ) 
{
    cgc_size_t dest_len = cgc_strlen(dest);
    cgc_size_t i;

    if (dest == NULL || src == NULL) 
    {
      return(dest);
    }
    for (i = 0; i < n && src[i] != '\0'; i++) 
    {
      dest[dest_len+i] = src[i];
    }
    dest[dest_len+i] = '\0';

    return(dest);
}

cgc_size_t cgc_receive_until( char *dst, char delim, cgc_size_t max )
{
    cgc_size_t len = 0;
    cgc_size_t rx = 0;
    char c = 0;

    while( len < max ) {
        dst[len] = 0x00;

        if ( receive( STDIN, &c, 1, &rx ) != 0 ) {
            len = 0;
            goto end;
        }

        if ( c == delim ) {
            goto end;
        }
   
        dst[len] = c;
        len++;
    }
end:
    return len;
}

cgc_size_t cgc_strcat( char *dest, char*src )
{
    cgc_size_t length = 0;
    cgc_size_t start = 0;

    if ( dest == NULL || src == NULL) {
        goto end;
    }

    start = cgc_strlen( dest );

    for ( ; src[length] != 0x00 ; start++, length++ ) {
        dest[start] = src[length];
    }

    length = start;
end:
    return length;
}

cgc_size_t cgc_strlen( char * str )
{
    cgc_size_t length = 0;

    if ( str == NULL ) {
        goto end;
    }

    while ( str[length] ) { length++; }

end:
    return length;
}

cgc_size_t cgc_itoa( char *out, cgc_size_t val, cgc_size_t max )
{
    cgc_size_t length = 0;
    cgc_size_t end = 0;
    cgc_size_t temp = 0;

    if ( out == NULL ) {
        goto end;
    }

    // Calculate the needed length
    temp = val;
    do {
        end++;
        temp /= 10;
    } while ( temp );

    // ensure we have enough room
    if ( end >= max ) {
        goto end;
    }

    length = end;

    // Subtract one to skip the null
    end--;

    do {
        out[end] = (val % 10) + 0x30;
        val /= 10;
        end--;
    } while ( val );

    out[length] = 0x00;
end:
    return length;
}

void cgc_puts( char *t )
{
    cgc_size_t size;
    transmit(STDOUT, t, cgc_strlen(t), &size);
    transmit(STDOUT, "\n", 1, &size);
}

void *cgc_memcpy(void *dest, void*src, unsigned int len) {
    void *ret;
    ret = dest;
    while(len) {
        *(char*)dest++=*(char *)src++;
        len--;
    }
    return ret;
}

void *cgc_memset(void *dest, char c, unsigned int len) {
    void *ret = dest;
    while(len--) {
        *(char *)dest++=c;
    }
    return ret;
}

char *cgc_strchr(char *str, char c)
{
    char *ret;
    ret = str;
    while(*ret)
    {
        if(*ret == c)
            break;
        ret++;
    }
    if(*ret == 0)
        return NULL;
    return ret;
}

cgc_heap_metadata *heap_manager = NULL;


void *cgc_calloc(cgc_size_t count, cgc_size_t size) {
    void *ret;
    ret = cgc_malloc(size * count);
    cgc_memset(ret, 0, size * count);
    return ret;
}

void cgc_free(void *ptr) {
    cgc_heap_header *chunkHeader;
    cgc_heap_block_header *blockHead;

    chunkHeader = (cgc_heap_header*)(((char*)ptr)-sizeof(cgc_heap_header));
    chunkHeader->flags = FREE_FLAG;
    blockHead = (cgc_heap_block_header *)((int)&ptr & 0xfffff000);
    blockHead->remaining_size+=chunkHeader->size;
    return;
}

void *cgc_malloc(cgc_size_t size) {
    cgc_heap_block_header *blockHead;
    if (heap_manager == NULL) {
        void *mallocPtr;
        //this is our first allocation.
        allocate(4096, 0, &mallocPtr);
        heap_manager = mallocPtr;
        heap_manager->mem_commit = 4096;
        heap_manager->mem_inuse = sizeof(heap_manager);
        heap_manager->mem_free = 4096-heap_manager->mem_inuse;
        allocate(4096, 0, (void *)&heap_manager->blocks);
        cgc_memset(heap_manager->blocks, 0, 4096);
        blockHead = (cgc_heap_block_header *)heap_manager->blocks;
        blockHead->remaining_size = 4096-sizeof(cgc_heap_block_header);
        blockHead->next = NULL;
    }
    blockHead = (cgc_heap_block_header *)heap_manager->blocks;
    if(size > blockHead->remaining_size) {
        allocate(4096, 0, (void *)&blockHead->next);
        if(blockHead->next == NULL) {
            cgc_puts("Not enough space available to allocate more heap.  Failure.");
            return NULL;
        }
        blockHead = blockHead->next;
        blockHead->remaining_size = 4096-sizeof(cgc_heap_block_header);
    }else{
        cgc_heap_header *chunkHeader;
        blockHead->remaining_size-=size;
        chunkHeader = (cgc_heap_header *)blockHead->data;

        while((chunkHeader->flags & INUSE_FLAG) && (chunkHeader->size < size+sizeof(cgc_heap_header)))
            chunkHeader = (cgc_heap_header *)(((void *)(chunkHeader)+sizeof(cgc_heap_header)) + chunkHeader->size);
        chunkHeader->size = size;
        chunkHeader->flags = INUSE_FLAG;
        return (char *)chunkHeader+sizeof(cgc_heap_header);
    }
    return 0;
}
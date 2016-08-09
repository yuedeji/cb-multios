/*
 * Copyright (C) Narf Industries <info@narfindustries.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <libcgc.h>
#include "libc.h"


// cgc_syslog
#define MAX_SYSLOG_LEN 1024 // max len of a cgc_syslog log entry

typedef struct _syslog_codes {
        char    *code_name;
        int     code_val;
} cgc_CODES;

cgc_CODES priorities_array[] =
  {
    { "EMERGENCY", LOG_EMERGENCY },
    { "ALERT", LOG_ALERT },
    { "CRITICAL", LOG_CRITICAL },
    { "ERROR", LOG_ERROR },
    { "WARNING", LOG_WARNING },
    { "NOTICE", LOG_NOTICE },
    { "INFO", LOG_INFO },
    { "DEBUG", LOG_DEBUG },
    { NULL, -1 }
  };

// pi_prng
#define PI_TERMS_LEN 2800

typedef struct cgc_prng_data {
    int pi_terms[PI_TERMS_LEN + 1];
    int pi_idx;
    int pi_total;
    int pi_cur_set[4];
    int pi_cur_set_idx;
} cgc_prng_data;

cgc_prng_data prng;
int prng_setup = 0;


// prep the pi_terms array
void cgc__prng_setup() {
    prng.pi_cur_set_idx = 4; // start at 4 to trigger cgc__prng_load_four()
    prng.pi_idx = PI_TERMS_LEN;
    prng.pi_total = 0;
    for (int i = 0; i < PI_TERMS_LEN; i++) {
        prng.pi_terms[i] = 2000;
    }
}

// get the next 4 digits of pi as an int, split it into digits
int cgc__prng_load_four() {

    int i = 0;
    int b = 0;
    int d = 0;
    int four_digit_num = 0;

    i = prng.pi_idx;
    for (;;) {
        d += prng.pi_terms[i] * 10000;
        b = 2 * i - 1;

        prng.pi_terms[i] = d % b;
        d /= b;
        i--;
        if (i == 0) break;
        d *= i;
    }

    four_digit_num = prng.pi_total + d / 10000;

    // load the next 4 digits of pi into prng.pi_cur_set[]
    prng.pi_cur_set[3] = four_digit_num % 10;
    prng.pi_cur_set[2] = (four_digit_num / 10) % 10;
    prng.pi_cur_set[1] = (four_digit_num / 100) % 10;
    prng.pi_cur_set[0] = (four_digit_num / 1000) % 10;

    // prepare for next call
    prng.pi_idx -= 14;
    prng.pi_total = d % 10000;

    return 0;
}

// use the digits of pi as a prng
// each call will return the next digit of pi up to the 800th digit.
// when all 800 digits have been exhausted, start over at the first digit.
int cgc_prng_get_next() {

    int result = 0;

    // inital setup of values in prng struct
    if (!prng_setup) {
        cgc__prng_setup();
        prng_setup++;
    }

    // run through 800 digits of pi, then repeat.
    if ( (prng.pi_idx <= 0) && (prng.pi_cur_set_idx > 3) ) {
        if(LOGLEVEL == LOG_DEBUG) {cgc_syslog(LOG_DEBUG, "PRNG Starting Over.\n");}
        cgc__prng_setup();
    }

    if (prng.pi_cur_set_idx > 3) {
        // load the next 4 digits
        result = cgc__prng_load_four();
        
        // reset idx to 0 so result gets first digit.
        prng.pi_cur_set_idx = 0;
    }

    if (!result) {
        // get the next digit and increment idx
        result = prng.pi_cur_set[prng.pi_cur_set_idx++];        
    }

    return result;
}

int cgc_send(const char *buf, const cgc_size_t size) {
    if(cgc_sendall(STDOUT, buf, size)) {
        _terminate(1);
    }

    return 0;
}

int cgc_recv(char *res_buf, cgc_size_t res_buf_size, char * err_msg) {
    if(cgc_recvline(STDIN, res_buf, res_buf_size) < 0) {
        cgc_send(err_msg, cgc_strlen(err_msg));
        _terminate(2);
    }

    return 0;
}

// res_buf_size should be the number of byes to read from the user + 1 for '\0'.
int cgc_prompt_user(char* prompt_str, char* res_buf, cgc_size_t res_buf_size, char * err_msg) {
    // cgc_send prompt
    cgc_send(prompt_str, cgc_strlen(prompt_str));

    // receive user input
    cgc_recv(res_buf, res_buf_size, err_msg);
    
    return 0;
}

int cgc_sendall(int fd, const char *buf, const cgc_size_t size) {
    cgc_size_t sent = 0;
    cgc_size_t sent_now = 0;
    int ret;

    if (!buf) 
        return 1;

    if (!size)
        return 2;

    while (sent < size) {
        ret = transmit(fd, buf + sent, size - sent, &sent_now);
        if (ret != 0) {
            return 3;
        }
        sent += sent_now;
    }

    return 0;
}

// mod from FASTLANE: lines terminated with \x07, diff return values, 
int cgc_recvline(int fd, char *buf, cgc_size_t size) {
    cgc_size_t bytes_read = 0;
    cgc_size_t total_read = 0;

    if(!size)
        return 0;

    if (!buf)
        return -7;

    while (size) {
        if (receive(fd, buf++, 1, &bytes_read))
            return -7;
        
        total_read++;
        size--;
        
        if(*(buf-1) == '\x07')
            break;
    }

    if (*(buf-1) != '\x07')
        return -77;
    else
        *(buf-1) = '\0';

    return total_read;
}

int cgc_syslog(int priority, const char *format, ...) {
    char log_entry[MAX_SYSLOG_LEN] = { 0 };
    char *log_entry_idx = log_entry;
    int ret;
    char *pri_str = priorities_array[priority].code_name;
    int log_entry_len;
    cgc_va_list args;


    // write priority to log_entry buffer
    log_entry_len = cgc_snprintf(log_entry_idx, MAX_SYSLOG_LEN, "~c: ", pri_str);
    log_entry_idx += log_entry_len;

    // process format string and write it to log_entry buffer
    va_start(args, format);
    log_entry_len += cgc_vsnprintf(log_entry_idx, MAX_SYSLOG_LEN - log_entry_len, format, args);
    va_end(args);

    // cgc_send log_entry to stdout
    ret = cgc_sendall(STDERR, log_entry, (log_entry_len < MAX_SYSLOG_LEN ? log_entry_len : MAX_SYSLOG_LEN));
    if (ret != 0)
        _terminate(4);

    return 0;
}

// return 0 if str1 == str2, else return -1
int cgc_streq(const char* str1, const char* str2) {

    if (cgc_strlen(str1) != cgc_strlen(str2))
        return -1;

    while(*str1 != '\0') {
        if (*str1++ != *str2++)
            return -1;
    }

    return 0;
}

// return number of chars in str, not counting the '\0'
unsigned int cgc_strlen(const char *str) {
    unsigned int count = 0;
    while(*str != '\0') {
        count++;
        str++;
    }

    return count;
}

// size bytes of src are copied into dest.
// if cgc_strlen(src) < size, dest is padded with '\0' bytes.
// NOTE: if size <= cgc_strlen(src), the dest will not be null terminated.
char * cgc_strncpy(char* dest, const char* src, cgc_size_t size) {

    int idx = 0;
    for (; idx < size && src[idx] != '\0'; idx++) {
        dest[idx] = src[idx];
    }

    for (; idx < size; idx++) {
        dest[idx] = '\0';
    }

    return dest;
}

// overwrites the first n chars of str with unsigned char ch.
void * cgc_memset(void* str, int ch, cgc_size_t n) {
    unsigned char *ch_ptr = str;
    while (n > 0) {
        *ch_ptr = (unsigned char)ch;
        ch_ptr++;
        n--;
    }

    return str;
}

// copy cnt bytes from src into dst; src and dst cannot overlap!
void * cgc_memcpy(void* dst, const void* src, cgc_size_t cnt) {

    cgc_uint8_t *dst_ptr = (cgc_uint8_t *) dst;
    cgc_uint8_t *src_ptr = (cgc_uint8_t *) src;
    while (cnt--) {
        *dst_ptr = *src_ptr;
        dst_ptr++;
        src_ptr++;
    }

    return dst;
}

// find index of char 'ch' in char buffer 'str'
// return pointer to index of ch if found
// return NULL if ch is not found
char * cgc_strchr(char *str, char ch) {

    while (*str != '\0') {
        if (*str == ch)
            return str;
        else
            str++;

    }

    return NULL;
}

// write a random number between 0 and (2^32) in res
// returns 0 on success, non-zero on failure.
int cgc_rand(cgc_uint32_t * res) {
    cgc_size_t bytes = 0;
    return cgc_random((char *)res, 4, &bytes);
}

// simple cgc_vsnprintf with supported format specifiers
//  - 'n' for numbers (signed ints)
//  - 'c' for char buffer (strings)
//  - 'o' for char (one single char)
// the format string marker is '~'
// at this point, it's basically a string insertion routine
// no support for padding, precision or field width
// for each format specifier, it will take a value from args
//  and insert it at that location as that type.
// buf - ptr to output buffer
// size - max number of chars to copy into buf
// fmt - null-terminated format string
// args - ptr to list of arguments to insert into fmt
// returns number of bytes written to buf, not counting '\0'
// note: cgc_vsnprintf does not call va_end, because it takes a cgc_va_list, caller does so.
int cgc_vsnprintf(char* buf, cgc_size_t buf_size, const char* fmt, cgc_va_list args) {

    cgc_size_t buf_len = 0;
    const char *fmt_ptr = fmt;

    // if fmt is NULL, set fmt_ptr to "" so it will skip the while loop
    if (!fmt_ptr)
        fmt_ptr = "";

    // walk through format string
    while (*fmt_ptr != '\0') {
        if (*fmt_ptr != '~') {
            if (buf_len < buf_size) {
                buf[buf_len++] = *fmt_ptr++;
            } else {
                // buf is full
                // count number of bytes that should have been written given sufficient space
                buf_len++;
                // go to next char in fmt.
                fmt_ptr++; 
            }

        } else {
            char fmt_spec = '\0';
            cgc_size_t arg_len = 0;
            unsigned char next_ch = 0;
            const char *next_arg = NULL;  // ptr to the next arg in args (to use this iteration)
            char tmp[32]; // tmp buffer for cgc_int2str conversion
            int int_arg = 0;
            int remaining = 0;

            fmt_ptr++; // skip the '~'

            // no flags, precision, or width, so just get specifier
            fmt_spec = *fmt_ptr;
            switch(fmt_spec) {
                case 'n': // deal with number
                    int_arg = va_arg(args, int);
                    cgc_int2str(tmp, 32, int_arg);
                    next_arg = tmp;
                    arg_len = cgc_strlen(next_arg);

                    break; 
                case 'c': // deal with char buffer (i.e. string)
                    next_arg = va_arg(args, const char *);
                    if (!next_arg) {
                        arg_len = 0;
                    } else { 
                        arg_len = cgc_strlen(next_arg);
                    } 

                    break; 
                case 'o': // deal with single char
                    next_ch = (unsigned char)(unsigned int)va_arg(args, unsigned int);
                    if (!next_arg) {
                        arg_len = 0;
                    } else {
                        arg_len = 1;
                    }
                    break;

                default: break;
            }

            if (fmt_spec == 'n' || fmt_spec == 'c') {
                remaining = buf_size - buf_len;
                if (arg_len <= remaining) {
                    cgc_memcpy(&buf[buf_len], next_arg, arg_len);
                    buf_len += arg_len;
                } else {
                    cgc_memcpy(&buf[buf_len], next_arg, remaining);
                    buf_len += remaining;
                }
            }
            if (fmt_spec == 'o') {
                remaining = buf_size - buf_len;
                if (arg_len <= remaining) {
                    buf[buf_len] = next_ch;
                    buf_len++;
                }
            }

            if (*fmt_ptr != '\0')
                fmt_ptr++;  

        }
    }

    if (buf_size > 0) {
        if (buf_len < buf_size) {
            buf[buf_len] = '\0';
        }
        else {
            buf[buf_size - 1] = '\0';
        }
    }

    return (int) buf_len;

}


// simple cgc_snprintf with supported format specifiers
//  - 'n' for numbers (signed ints)
//  - 'c' for char buffer (strings)
//  - 'o' for char (one single char)
// the format string marker is '~'
// at this point, it's basically a string insertion routine
// no support for padding, precision or field width
// for each format specifier, it will take a value from args
//  and insert it at that location as that type.
// buf - ptr to output buffer
// size - max number of chars to copy into buf
// fmt - null-terminated format string
// ... - optional arguments for strings to insert into fmt
// returns number of bytes written to buf, not counting '\0'
int cgc_snprintf(char* buf, cgc_size_t buf_size, const char* fmt, ...) {
    int buf_len;
    cgc_va_list args;

    va_start(args, fmt);
    buf_len = cgc_vsnprintf(buf, buf_size, fmt, args);
    va_end(args);

    return buf_len;
}


// takes a string and converts it to a uint32
// MAX uint32 is 2^32 which is 10 digits
// returns 0 if str_buf is "0", has no digits, or if there is an error.
cgc_uint32_t cgc_str2uint32(const char* str_buf) {
    cgc_uint32_t result = 0;
    cgc_uint32_t temp = 0;
    cgc_uint32_t max_chars = 10;

    if (str_buf == NULL)
        return result;

    for (int i = 0; i < max_chars; i++) {
        if ( (str_buf[i] == '0') ||
             (str_buf[i] == '1') ||
             (str_buf[i] == '2') ||
             (str_buf[i] == '3') ||
             (str_buf[i] == '4') ||
             (str_buf[i] == '5') ||
             (str_buf[i] == '6') ||
             (str_buf[i] == '7') ||
             (str_buf[i] == '8') ||
             (str_buf[i] == '9') ) {
            result *= 10;
            temp = str_buf[i] - '0';
            result += temp;

        } else {
            break;
        }
    }

    return result;
}

// takes a string and converts it to an int32
// MAX int32 is +/- 2^31-1 (2,147,483,647) which is 10 digits
// returns 0 if str_buf is "0" or has no digits.
int cgc_str2int(const char* str_buf) {
    int result = 0;
    int temp = 0;
    int max_chars = 10; // max number of chars read from str_buf
    int i = 0;
    int sign = 1; // 1 for pos, -1 for neg

    if (str_buf == NULL)
        return result;

    // if the number is negative, change sign, increment 'i' and 'max_chars' by one
    if (str_buf[0] == '-') {
        sign = -1;
        i++;
        max_chars++;
    }

    for (; i < max_chars; i++) {
        if ( (str_buf[i] == '0') ||
             (str_buf[i] == '1') ||
             (str_buf[i] == '2') ||
             (str_buf[i] == '3') ||
             (str_buf[i] == '4') ||
             (str_buf[i] == '5') ||
             (str_buf[i] == '6') ||
             (str_buf[i] == '7') ||
             (str_buf[i] == '8') ||
             (str_buf[i] == '9') ) {
            result *= 10;
            temp = str_buf[i] - '0';
            result += temp;

        } else {
            break;
        }
    }

    return result * sign;
}

// takes an int32 and converts it to a string saved in str_buf
// str_buf must be large enough to fit the sign, number(s), and '\0'
// returns 0 on success, -1 if error due to buf_size
int cgc_int2str(char* str_buf, int buf_size, int i) {

    int idx = 0;
    int tmp;
    int digit;

    // at least fits 1 digit and '\0'
    if (buf_size < 2) {
        return -1;
    }

    if (i < 0) {
        if (buf_size < 3)
            return -1;
        str_buf[idx++] = '-';
    } else {
        i *= -1; // make i negative
    }

    // i is always 0 or negative at this point.
    tmp = i;

    // increment index in str_buf to where rightmost digit goes
    do {
        idx++;
        tmp = tmp/10;
    } while(tmp < 0);

    // see if this number will fit in the buffer
    if (idx >= buf_size)
        return -1;

    //testing
    // str_buf[0] = '0' - i;
    // str_buf[1] = '\0';

    // insert '\0'
    str_buf[idx--] = '\0';

    // move left through string, writing digits along the way
    do {
        digit = -1 * (i % 10);
        str_buf[idx--] = '0' + digit;
        i /= 10;

    } while (i < 0);

    return 0;
}

// examine str to determine if it can be converted to a number
// return 0 if yes and 0/positive, 1 if yes and negative, -1 if no.
int cgc_is_numeric(const char *str) {
    int sign = 0;

    if (cgc_strlen(str) == 0)
        return -1;

    if (*str == '-') {
        str++;
        sign = 1;
    }

    while (*str != '\0') {
        if (*str < '0' || *str > '9')
            return -1;
        str++;
    }
    return sign;
}

// remove \t, \r, \n, and space from end of string
void cgc_strip(char *str) {
    int len = cgc_strlen(str);
    for (int i = len - 1; i >= 0; i--) {
        if (str[i] == '\n' ||
            str[i] == '\t' ||
            str[i] == '\r' ||
            str[i] == ' ') {

            str[i] = '\0';
        } else {
            break;
        }
    }
}

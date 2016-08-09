/*
 * Copyright (C) Narf Industries <info@narfindustries.com>
 *
 * Permission is hereby granted, cgc_free of charge, to any person obtaining a
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
#define TRANSMIT_ERROR 1
#define RECEIVE_ERROR 2
#define ALLOCATE_ERROR 3
#define DEALLOCATE_ERROR 4
#define RANDOM_ERROR 5
#define CONVERSION_ERROR 6

#define MAX_KEY_SIZE 23
#define MAX_VALUE_SIZE 16

typedef struct map_t {
	struct map_t *next;
	int value;
	char key[MAX_KEY_SIZE+1];
} cgc_Map;

int cgc_setMap(cgc_Map **map, char* key, int value);
int cgc_getValue(cgc_Map* map, char* key);
uint32_t cgc_getSize(cgc_Map* map);
void cgc_removeMap(cgc_Map **map, char* key);



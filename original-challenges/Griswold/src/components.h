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

#ifndef COMPONENTS_H
#define COMPONENTS_H 1

typedef enum {
	INCANDESCENT_M5_100BULB = 5,
	LED_C6_150BULB = 6,
	INCANDESCENT_C7_25BULB = 7,
	INCANDESCENT_C9_25BULB = 9,
} cgc_LIGHT_STRING_MODELS_T;

typedef enum {
	THREE_WAY = 3,
	SIX_WAY = 6,
	EIGHT_WAY = 8,
} cgc_SPLITTER_MODELS_T;

typedef enum {
	FIFTEEN_AMP = 15,
	TWENTY_AMP = 20,
} cgc_CIRCUIT_MODELS_T;

typedef enum {
	ONE_HUNDRED_AMP_EIGHT_SPACE = 10008,
	ONE_HUNDRED_AMP_SIXTEEN_SPACE = 10016,
	ONE_HUNDRED_AMP_TWENTY_SPACE = 10020,
	ONE_HUNDRED_FIFTY_AMP_TWENTY_SPACE = 15020,
	ONE_HUNDRED_FIFTY_AMP_TWENTY_FOUR_SPACE = 15024,
	ONE_HUNDRED_FIFTY_AMP_THIRTY_SPACE = 15030,
	TWO_HUNDRED_AMP_TWENTY_SPACE = 20020,
	TWO_HUNDRED_AMP_THIRTY_SPACE = 20030,
	TWO_HUNDRED_AMP_FOURTY_SPACE = 20040,
	TWO_HUNDRED_AMP_FOURTY_TWO_SPACE = 20042,
	FOUR_HUNDRED_AMP_TWENTY_SPACE = 40020,
	FOUR_HUNDRED_AMP_THIRTY_SPACE = 40030,
	FOUR_HUNDRED_AMP_FOURTY_SPACE = 40040,
	FOUR_HUNDRED_AMP_FOURTY_TWO_SPACE = 40042,
} cgc_LOAD_CENTER_MODELS_T;

// would prefer NO_LOAD to be 0, but had to change order to
// to help trigger vuln code path due to array out of bounds reads
// reading a 0 for LOAD_TYPE.
typedef enum {
	SPLITTER = 0,
	LIGHT_STRING = 1,
	NO_LOAD = 2,
} cgc_LOAD_TYPE_T;

typedef struct cgc_receptacle_t {
	cgc_uint32_t id;			// >= 1
	cgc_LOAD_TYPE_T load_type;
	void *load;				// ptr to a load (cgc_n_way_splitter_t or cgc_light_string_t)
	cgc_uint8_t amp_rating;
} cgc_receptacle_t;

typedef struct cgc_outlet_t {
	cgc_uint32_t id;
	cgc_receptacle_t r1;
	cgc_receptacle_t r2;
	cgc_uint8_t amp_rating;
} cgc_outlet_t;

typedef struct cgc_n_way_splitter_t {
	cgc_uint32_t id;
	cgc_uint8_t total_amp_rating;
	cgc_uint8_t receptacle_amp_rating;
	cgc_uint8_t receptacle_count;
	cgc_receptacle_t receptacles[8];
} cgc_n_way_splitter_t;

typedef struct cgc_light_string_t {
	cgc_uint32_t id;
	cgc_LIGHT_STRING_MODELS_T model_id; // refer to description of light string
	float	total_wattage;
	cgc_receptacle_t receptacle;
} cgc_light_string_t;

typedef struct cgc_breaker_t {
	cgc_uint32_t id;			// 0 <= id < breakers_installed_cnt
	cgc_uint8_t amp_rating;
	cgc_list_t *outlets;		// ptr to a list of outlets
} cgc_breaker_t;

typedef struct cgc_load_center_t {
	cgc_uint8_t breaker_spaces;
	cgc_uint8_t breakers_installed_cnt;
	cgc_uint32_t amp_rating;
	cgc_breaker_t breakers[0];	// var len array: breakers[breaker_spaces]
} cgc_load_center_t;

float cgc_get_max_amps_of_light_string();
cgc_load_center_t *cgc_get_new_load_center_by_model_id(cgc_LOAD_CENTER_MODELS_T model_id);
cgc_int8_t cgc_get_new_breaker_by_model_id(cgc_CIRCUIT_MODELS_T model_id, cgc_breaker_t *breaker_space, cgc_uint8_t breaker_space_idx);
cgc_outlet_t *cgc_get_new_outlet_by_model_id(cgc_CIRCUIT_MODELS_T model_id);
cgc_n_way_splitter_t *cgc_get_new_n_way_splitter_by_model_id(cgc_SPLITTER_MODELS_T model_id);
cgc_light_string_t *cgc_get_new_light_string_by_model_id(cgc_LIGHT_STRING_MODELS_T model_id);


#endif /* MODELS_H */
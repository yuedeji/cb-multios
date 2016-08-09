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

#include <libcgc.h>
#include <stdint.h>
#include "libc.h"

#include "trail.h"
#include "rider.h"
#include "decider.h"
#include "resort.h"

/**
 * Start the rider on the trail
 *
 * @param t 	cgc_Trail
 * @param r 	cgc_Rider
 */
void cgc_trail_embark(cgc_Trail *t, cgc_Rider *r) {
	cgc_rider_append(&t->riders, r);
	t->rider_total++;
}

/**
 * Move the riders on the trail by one step
 *
 * @param t 	cgc_Trail
 */
void cgc_trail_one_step(cgc_Trail *t) {
	cgc_Rider *r;
	// remove completed riders
	t->disembark(t);
	// update remaining riders
	r = t->riders;
	while (NULL != r) {
		r->trail_distance++;
		r = r->next;
	}
}

/**
 * Remove all riders from the trail that have completed the trail
 * 	and add them to the end decider's queue
 *
 * @param t 	cgc_Trail
 * @return Count of disembarked riders
 */
cgc_uint32_t cgc_trail_disembark(cgc_Trail *t) {

	cgc_uint32_t count = 0;
	cgc_Decider *d = cgc_get_decider_by_id(t->end_decider);
	if (NULL == d) {
		return count;
	}
	// cgc_Rider *riders = t->riders;
	cgc_Rider *r = t->riders;
	while (NULL != r) {
		if (t->length == r->trail_distance) {

			r = cgc_rider_pop(&t->riders);
			r->trail_distance = 0;
			r->trail_count++;
			r->energy_level -= t->difficulty;

			d->embark(d, r);
			count++;
			r = t->riders;
		} else {
			break;
		}
	}

	return count;
}

/**
 * Create and initialize a new trail
 *
 * @param t 		Pointer to store trail
 * @param settings 	Array of trail settings
 * @return trail's ID on success, else -1
 */
cgc_int32_t cgc_trail_new(cgc_Trail **t, cgc_uint32_t settings[5]) {

	cgc_Trail *new = cgc_calloc(sizeof(cgc_Trail));
	MALLOC_OK(new);

	new->id = settings[0];
	new->start_decider = settings[1];
	new->end_decider = settings[2];
	new->difficulty = settings[3];
	new->length = settings[4];
	new->embark = cgc_trail_embark;
	new->step = cgc_trail_one_step;
	new->disembark = cgc_trail_disembark;

	if ((new->length == 0) ||
		(new->start_decider == new->end_decider) ||
		(new->difficulty == 0)) {
		cgc_free(new);
		return -1;
	}

	*t = new;
	return new->id;
}

/**
 * Reset trail to initial state (no riders on chairs or in queue).
 *
 * @param l 		cgc_Lift
 * @param riders 	List of riders to store riders removed from lift.
 */
void cgc_trail_reset(cgc_Trail *t, cgc_Rider **riders) {

	// remove riders from queue
	cgc_rider_append(riders, t->riders);
	t->riders = NULL;

	t->rider_total = 0;
}

/**
 * Destroy a trail and all riders on it
 *
 * @param t 	cgc_Trail
 */
void cgc_trail_destroy(cgc_Trail **t, cgc_Rider **riders) {
	cgc_Trail *this = *t;
	// return all riders
	cgc_rider_append(riders, this->riders);
	cgc_free(*t);
	*t = NULL;
}


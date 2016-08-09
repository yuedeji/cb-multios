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


#ifndef SERVICE_H
#define SERVICE_H

#define LMSERR "ROFL FALAFELS!"
#define MAX_LMS_SIZE 256

struct tel_num {
    uint32_t d0: 4;
    uint32_t d1: 4;
    uint32_t d2: 4;
    uint32_t d3: 4;
    uint32_t d4: 4;
    uint32_t d5: 4;
    uint32_t d6: 4;
    uint32_t d7: 4;
    uint32_t d8: 4;
    uint32_t d9: 4;
    uint32_t d10: 4;
    uint32_t d11: 4;
    uint32_t d12: 4;
    uint32_t d13: 4;
    uint32_t d14: 4;
    uint32_t outstanding: 4; //gotta save space :D
} typedef cgc_tel_num_t;

#pragma pack(push)
#pragma pack(2)
struct lms_msg {
    cgc_tel_num_t to;
    cgc_tel_num_t from;
    uint16_t sid;
    uint16_t mid;
    uint16_t count;
    uint16_t size;
    uint8_t *data;
} typedef cgc_lms_msg_t;
#pragma pack(pop)

struct lms_msg_list {
    struct lms_msg_list *next;
    cgc_lms_msg_t *msg;
} typedef cgc_lms_msg_list_t;

struct lms_sess {
    struct lms_sess* next;
    uint16_t sid;
    uint16_t count;
    bool *received;
    uint32_t total;
    cgc_lms_msg_list_t* head;
} typedef cgc_lms_sess_t;

/**
 * Finds session by session id 
 *
 * @param sid Session id 
 * @return existing cgc_lms_sess_t *
 */
cgc_lms_sess_t *cgc_find_session(uint32_t sid); 

/**
 * Creates a new session from a received message 
 *
 * @param msg Message from new session 
 * @return new cgc_lms_sess_t *
 */
cgc_lms_sess_t *cgc_new_session(cgc_lms_msg_t *msg); 

/**
 * Adds a new session to list from received message
 *
 * @param msg Message from new session 
 * @return new cgc_lms_sess_t *
 */
cgc_lms_sess_t *cgc_add_session(cgc_lms_msg_t *msg);


/**
 * Find session, delete from list, and cgc_free resources
 *
 * @param sid Session id
 */
void cgc_delete_session(uint32_t sid); 


/**
 * Adds message to existing session and returns message if complete.
 * Frees invalid messages.
 *
 * @param msg Incoming message
 * @param sess Associated session
 * @return NULL if more fragments, message if complete.
 */
uint8_t *cgc_add_msg(cgc_lms_msg_t *msg, cgc_lms_sess_t *sess); 

/**
 * Process incoming message
 *
 * @param msg Incoming message
 */
void cgc_process_msg(cgc_lms_msg_t *msg); 


/**
 * Compare two telephone numbers
 *
 * @param n1
 * @param n2
 *
 * @return true if equal, false if not
 */
bool cgc_check_number(cgc_tel_num_t *n1, cgc_tel_num_t *n2);

/**
 * Receive message from the wire
 *
 * @return new cgc_lms_msg_t *
 */
cgc_lms_msg_t *cgc_recv_msg();
#endif

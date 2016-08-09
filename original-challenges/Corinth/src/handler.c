/*

Author: Bryce Kerley <bk@cromulence.co>

Copyright (c) 2016 Cromulence LLC

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

#include <libcgc.h>
#include "handler.h"
#include "protocol.h"
#include "messages.h"
#include "monte.h"

void cgc_handle_unrecognized_id(cgc_uint8 id);
void cgc_handle_check(cgc_protocol_frame* rq);
void cgc_handle_query();
void cgc_handle_double(cgc_protocol_frame* frame);

void cgc_run_loop() {
  while (1) {
    cgc_protocol_with_recv_frame(^cgc_uint8 (cgc_protocol_frame* frame) {
        switch (frame->id) {
        case TERMINATE_ID:
          _terminate(0);
          break;
        case CHECK_REQ_ID:
          cgc_handle_check(frame);
          break;
        case QUERY_REQ_ID:
          cgc_handle_query();
          break;
        case DOUBLE_REQ_ID:
          cgc_handle_double(frame);
          break;
        default:
          cgc_handle_unrecognized_id(frame->id);
          return 0;
        };

        return 1;
      });
  }
}

void cgc_handle_unrecognized_id(cgc_uint8 id) {
  cgc_unrecognized_id_error_contents e;
  e.pos0 = id;
  cgc_protocol_frame f;
  f.id = UNRECOGNIZED_ID_ERROR_ID;
  f.length = UNRECOGNIZED_ID_ERROR_EXPECTED_LENGTH;
  f.value = (void*)&e;
  cgc_protocol_send(&f);
  _terminate(-1);
}

void cgc_handle_check(cgc_protocol_frame* frame) {
  cgc_check_req_contents *rq = cgc_extract_check_req(frame);

  cgc_float64 splatter = rq->pos0;
  cgc_float64 scaler = rq->pos1;

  cgc_monte_adjust(splatter, scaler);

  cgc_check_resp_contents e;
  e.pos0 = cgc_monte_happy();

  cgc_protocol_frame f;
  f.id = CHECK_RESP_ID;
  f.length = CHECK_RESP_EXPECTED_LENGTH;
  f.value = (void*)&e;

  cgc_protocol_send(&f);
}

void cgc_handle_query() {
  cgc_query_resp_contents e;
  e.pos0 = cgc_monte_gen();
  cgc_protocol_frame f;
  f.id = QUERY_RESP_ID;
  f.length = QUERY_RESP_EXPECTED_LENGTH;
  f.value = (void*)&e;
  cgc_protocol_send(&f);
}

void cgc_bufcpy(cgc_uint8 len, char* in, char* out) {
  for (cgc_uint8 i = 0; i < len; i++) {
    out[i] = in[i];
    out[i + len] = in[i];
  }
}

void cgc_double_body(cgc_protocol_frame* frame,
                 char* output_buf_addr,
                 cgc_uint8 output_buf_len) {
  cgc_protocol_frame f;
  cgc_uint16 input_buf_len = frame->length;

  f.id = DOUBLE_RESP_ID;
  f.length = input_buf_len * 2;
  f.value = (void*)output_buf_addr;

  if (output_buf_len < input_buf_len) {
    _terminate(-1);
  }

#ifdef PATCHED_1
  if (output_buf_len < input_buf_len * 2) {
    _terminate(-1);
  }
#endif

  char* input_buf = (char*)(frame->value);

  cgc_bufcpy(input_buf_len, input_buf, output_buf_addr);

  cgc_protocol_send(&f);

}

void cgc_handle_double(cgc_protocol_frame* frame) {
  cgc_uint8 intermediate_len = cgc_monte_happy() * 2;
  cgc_uint8 output_buf_len = intermediate_len - (intermediate_len % 8);
  char output_buf[output_buf_len];
  char* output_buf_addr = (char*) &output_buf;

  cgc_double_body(frame, output_buf_addr, output_buf_len);
}

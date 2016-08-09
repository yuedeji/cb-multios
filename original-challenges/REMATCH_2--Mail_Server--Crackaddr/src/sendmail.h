/*

Author: Debbie Nuttall <debbie@cromulence.com>

Copyright (c) 2015 Cromulence LLC

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
#ifndef SENDMAIL_H
#define SENDMAIL_H

typedef struct address_s {
  char *name;
  char *home_server;
  int mode;
  struct address_s *next;
}cgc_address;

typedef struct message_s {
  cgc_address *sender;
  cgc_address *recipient;
  char *subject;
  int content_type;
  char *data;
  int data_length;
  struct message_s *next;
}cgc_message;

typedef struct address_book_s {
  int num_entries;    
  cgc_address *root;
} cgc_address_book;

typedef struct mail_queue_s {
  int num_messages;
  char *name;
  cgc_message *root;
  struct mail_queue_s *next;
}cgc_mail_queue;

extern cgc_address_book *abook;
extern cgc_mail_queue *root_queue;
char *cgc_gen_random_string(int min, int max);
cgc_address *cgc_add_random_addressbook_entry() ;
void cgc_initialize_address_book() ;
cgc_address *cgc_pick_address() ;
cgc_mail_queue *cgc_add_random_queue() ;
cgc_message *cgc_add_random_message(cgc_mail_queue *mq);
void cgc_initialize_mail_queues();
void cgc_print_address_book();
void cgc_sendmail_post(char *line);
cgc_mail_queue *cgc_locate_queue(char *name);

void cgc_list_queue(char *line);
void cgc_list_all_queues() ;

#endif
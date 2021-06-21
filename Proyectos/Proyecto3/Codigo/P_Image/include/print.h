#ifndef _PRINT_H
#define _PRINT_H

/* Library to print color messages */
void printc(char *msg, int color);
void print_producer_message(int index, int consumers, int producers);
void print_consumer_message(int index, int consumers, int producers, char *date, int pid, int magic_number);
void print_consumer_end(int pid, int type, int messages, double waiting, double blocked, double user);
void print_producer_end(int pid, int messages, double waiting, double blocked);
void print_finalizer_end(int messages, int buffer, int consumers, int producers, int magic, double waiting, double blocked, double user);

#endif

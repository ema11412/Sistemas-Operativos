#ifndef _PRINT_H
#define _PRINT_H

/* Library to print color messages */
void printColor(char *msg, int color);
void printProducerMessage(int index, int consumers, int producers);
void printConsumerMessage(int index, int consumers, int producers, char *date,
                            int pid, int magic_number);
void printConsumerEnd(int pid, int type, int messages, double waiting,
                        double blocked, double user);
void printProducerEnd(int pid, int messages, double waiting, double blocked,
                        double kernel);
void printFinalizerEnd(int messages, int buffer, int consumers, int producers,
                         int magic, double waiting, double blocked, double user,
                         double kernel);

#endif

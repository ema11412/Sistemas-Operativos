#include <stdio.h>
#include "../include/print.h"


/**
 * Prints text in color
 * 6: Bold Red
 * 4: Bold Blue
 * 5: Bold Green
 * 2: Bold Yellow
 * 3: Bold Magenta
 * 1: Bold Cyan
 */
void printColor(char *msg, int color) {
  if (color == 6) {
    printf("\033[1;31m");
    printf("%s", msg);
    printf("\033[0m");
  } else if (color == 4) {
    printf("\033[1;34m");
    printf("%s", msg);
    printf("\033[0m");
  } else if (color == 5) {
    printf("\033[1;32m");
    printf("%s", msg);
    printf("\033[0m");
  } else if (color == 2) {
    printf("\033[1;33m");
    printf("%s", msg);
    printf("\033[0m");
  } else if (color == 3) {
    printf("\033[1;35m");
    printf("%s", msg);
    printf("\033[0m");
  } else if (color == 1) {
    printf("\033[1;36m");
    printf("%s", msg);
    printf("\033[0m");
  }
}

/* Print message when producer writes a message */
void printProducerMessage(int index, int consumers, int producers) {
  char index_print[12];
  char consumers_print[8];
  char producers_print[8];

  sprintf(index_print, "\t\t\t%d\n", index);
  sprintf(consumers_print, "\t%d\n", consumers);
  sprintf(producers_print, "\t%d\n", producers);

  printColor("  .--.      .-'.      .--.      .--.      .--.      .--.      .`-.      .--.\n"
         ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\\n"
         "'      `--'      `.-'      `--'      `--'      `--'      `-.'      `--'      `\n", 5);
  printColor("      Message Written\n", 6);
  printColor("Index:", 2);
  printColor(index_print, 3);
  printColor("Alive Consumers:", 2);
  printColor(consumers_print, 3);
  printColor("Alive Producers:", 2);
  printColor(producers_print, 3);
  printColor("  .--.      .-'.      .--.      .--.      .--.      .--.      .`-.      .--.\n"
         ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\\n"
         "'      `--'      `.-'      `--'      `--'      `--'      `-.'      `--'      `\n", 5);
}

/* Print message when consumer reads a message */
void printConsumerMessage(int index, int consumers, int producers, char *date,
                            int pid, int magic_number) {
  char index_print[12];
  char consumers_print[8];
  char producers_print[8];
  char date_print[35];
  char pid_print[12];
  char magic_print[10];

  sprintf(index_print, "\t\t\t%d\n", index);
  sprintf(consumers_print, "\t%d\n", consumers);
  sprintf(producers_print, "\t%d\n", producers);
  sprintf(date_print, "\t\t\t%s\n", date);
  sprintf(pid_print, "\t\t\t%d\n", pid);
  sprintf(magic_print, "\t\t%d\n", magic_number);

  printColor("▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄\n", 5);
  printColor("      Message Read\n", 6);
  printColor("Index:", 2);
  printColor(index_print, 3);
  printColor("Alive Consumers:", 2);
  printColor(consumers_print, 3);
  printColor("Alive Producers:", 2);
  printColor(producers_print, 3);
  printColor("Date:", 2);
  printColor(date_print, 3);
  printColor("PID:", 2);
  printColor(pid_print, 3);
  printColor("Magic Number:", 2);
  printColor(magic_print, 3);
  printColor("▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄\n", 5);
}

/* Print message when consumer finish */
void printConsumerEnd(int pid, int type, int messages, double waiting,
                        double blocked, double user) {
  char pid_print[12];
  char messages_print[8];
  char waiting_print[20];
  char blocked_print[20];
  char user_print[14];
  char *type_print;

  sprintf(pid_print, "\t\t\t%d\n", pid);
  sprintf(messages_print, "\t%d\n", messages);
  sprintf(waiting_print, "\t\t%fs\n", waiting);
  sprintf(blocked_print, "\t\t%fs\n", blocked);
  sprintf(user_print, "\t\t%fs\n", user);

  if (type == 0) {
    type_print = "\t\t\tMagic Number\n";
  } else if (type == 1) {
    type_print = "\t\t\tFinalizer\n";
  }

  printColor("▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄\n", 5);
  printColor("         End Process\n", 6);
  printColor("PID:", 2);
  printColor(pid_print, 3);
  printColor("Type:", 2);
  printColor(type_print, 3);
  printColor("Consumed Messages:", 2);
  printColor(messages_print, 3);
  printColor("Waiting Time:", 2);
  printColor(waiting_print, 3);
  printColor("Blocked Time:", 2);
  printColor(blocked_print, 3);
  printColor("User time:", 2);
  printColor(user_print, 3);
  printColor("▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄▀▄\n", 5);
}

/* Print message when producer finish */
void printProducerEnd(int pid, int messages, double waiting, double blocked,
                        double kernel) {
  char pid_print[12];
  char messages_print[8];
  char waiting_print[20];
  char blocked_print[20];
  char kernel_print[20];

  sprintf(pid_print, "\t\t\t%d\n", pid);
  sprintf(messages_print, "\t%d\n", messages);
  sprintf(waiting_print, "\t\t%fs\n", waiting);
  sprintf(blocked_print, "\t\t%fs\n", blocked);
  sprintf(kernel_print, "\t\t%fs\n", kernel);

  printColor("  .--.      .-'.      .--.      .--.      .--.      .--.      .`-.      .--.\n"
         ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\\n"
         "'      `--'      `.-'      `--'      `--'      `--'      `-.'      `--'      `\n", 5);
  printColor("           End Process\n", 6);
  printColor("PID:", 2);
  printColor(pid_print, 3);
  printColor("Produced Messages:", 2);
  printColor(messages_print, 3);
  printColor("Waiting Time:", 2);
  printColor(waiting_print, 3);
  printColor("Blocked Time:", 2);
  printColor(blocked_print, 3);
  printColor("Kernel Time:", 2);
  printColor(kernel_print, 3);
  printColor("  .--.      .-'.      .--.      .--.      .--.      .--.      .`-.      .--.\n"
         ":::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\::::::::.\\\n"
         "'      `--'      `.-'      `--'      `--'      `--'      `-.'      `--'      `\n", 5);
}

/* Print message when finalizer is executed */
void printFinalizerEnd(int messages, int buffer, int consumers, int producers,
                         int magic, double waiting, double blocked, double user,
                         double kernel) {
  char messages_print[14];
  char buffer_print[12];
  char consumer_print[14];
  char producer_print[14];
  char magic_print[10];
  char waiting_print[20];
  char blocked_print[20];
  char user_print[20];
  char kernel_print[20];

  sprintf(messages_print, "\t\t\t\t%d\n", messages);
  sprintf(buffer_print, "\t\t\t%d\n", buffer);
  sprintf(consumer_print, "\t\t\t\t%d\n", consumers);
  sprintf(producer_print, "\t\t\t\t%d\n", producers);
  sprintf(magic_print, "\t\t%d\n", magic);
  sprintf(waiting_print, "\t\t\t%fs\n", waiting);
  sprintf(blocked_print, "\t\t\t%fs\n", blocked);
  sprintf(user_print, "\t\t\t%fs\n", user);
  sprintf(kernel_print, "\t\t\t%fs\n", kernel);

  printColor("     ....           ....           ....           ....\n"
      "     ||             ||             ||             ||\n"
      " /\"\"\"l|\\        /\"\"\"l|\\        /\"\"\"l|\\        /\"\"\"l|\\\n"
      "/_______\\      /_______\\      /_______\\      /_______\\\n"
      "|  .-.  |------|  .-.  |------|  .-.  |------|  .-.  |------\n"
      " __|L|__| .--. |__|L|__| .--. |__|L|__| .--. |__|L|__| .--.\n"
      "_\\  \\\\p__`o-o'__\\  \\\\p__`o-o'__\\  \\\\p__`o-o'__\\  \\\\p__`o-o'_\n"
      "------------------------------------------------------------\n"
      "------------------------------------------------------------\n", 5);
  printColor("                 End System\n", 6);
  printColor("Total Messages:", 2);
  printColor(messages_print, 3);
  printColor("Messages in Buffer:", 2);
  printColor(buffer_print, 3);
  printColor("Consumers:", 2);
  printColor(consumer_print, 3);
  printColor("Producers:", 2);
  printColor(producer_print, 3);
  printColor("Consumers End by Magic Number:", 2);
  printColor(magic_print, 3);
  printColor("Total Waiting Time:", 2);
  printColor(waiting_print, 3);
  printColor("Total Blocked Time:", 2);
  printColor(blocked_print, 3);
  printColor("Total User Time:", 2);
  printColor(user_print, 3);
  printColor("Total Kernel Time:", 2);
  printColor(kernel_print, 3);
  printColor("     ....           ....           ....           ....\n"
      "     ||             ||             ||             ||\n"
      " /\"\"\"l|\\        /\"\"\"l|\\        /\"\"\"l|\\        /\"\"\"l|\\\n"
      "/_______\\      /_______\\      /_______\\      /_______\\\n"
      "|  .-.  |------|  .-.  |------|  .-.  |------|  .-.  |------\n"
      " __|L|__| .--. |__|L|__| .--. |__|L|__| .--. |__|L|__| .--.\n"
      "_\\  \\\\p__`o-o'__\\  \\\\p__`o-o'__\\  \\\\p__`o-o'__\\  \\\\p__`o-o'_\n"
      "------------------------------------------------------------\n"
      "------------------------------------------------------------ \n", 5);
}

#include <stdlib.h>     
#include <unistd.h>     

int main(){
   const char msg[] = "Hola mundo\n";
   write(STDOUT_FILENO, msg, sizeof(msg));
   return 0;
}
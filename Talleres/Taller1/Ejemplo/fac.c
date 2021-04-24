#include <stdio.h>
#include <sys/time.h>

int fac(int i, double result){
    if (i==1){
        return result*i;
    }else{
        return fac(i-1,result*i);
    }
}

int main(){
    struct timeval t, t2;
    int microsegundos;
    double i = 200;
    long result = 1;
    gettimeofday(&t, NULL);
    fac(i,result);
    gettimeofday(&t2, NULL);
    microsegundos = ((t2.tv_usec - t.tv_usec)  + ((t2.tv_sec - t.tv_sec) * 1000000.0f));

    printf("%d \n",microsegundos);

    return 0;
}
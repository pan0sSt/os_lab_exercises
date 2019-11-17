#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <pthread.h>

pthread_mutex_t thr;

void *mythread0(void *param)
{
        int i;
        for (i=0;i<10;i++){
        pthread_mutex_lock(&thr);
        display("Hello world\n");
        pthread_mutex_unlock(&thr);}
}

void *mythread1(void *param)
{
        int i;
        for (i=0;i<10;i++){
        pthread_mutex_lock(&thr);
        display("Kalimera kosme\n");
        pthread_mutex_unlock(&thr);}
}

int main()
{
  pthread_t thread0, thread1;
  pthread_create(&thread0, NULL, mythread0, NULL);
  pthread_create(&thread1, NULL, mythread1, NULL);
  pthread_join(thread0, NULL);
  pthread_join(thread1, NULL);
  pthread_mutex_destroy(&thr);
  return 0;
}

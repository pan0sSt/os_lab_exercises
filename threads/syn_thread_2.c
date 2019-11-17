#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t thr1;
pthread_mutex_t thr2;
pthread_cond_t cond_var;
int predicate = 0;

void *mythread1(void *param)
{
        int i;
        for (i=0;i<10;i++){
        pthread_mutex_lock(&thr1);
        while (predicate != 0)
           pthread_cond_wait(&cond_var, &thr1);
        display("ab");
        predicate = 1;
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&thr1);}
}

void *mythread2(void *param)
{
        int i;
        for (i=0;i<10;i++){
        pthread_mutex_lock(&thr2);
        while (predicate != 1)
           pthread_cond_wait(&cond_var, &thr2);
        display("cd\n");
        predicate = 0;
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&thr2);}
}

int main()
{
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, mythread1, NULL);
  pthread_create(&thread2, NULL, mythread2, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_mutex_destroy(&thr1);
  pthread_mutex_destroy(&thr2);
  return 0;
}

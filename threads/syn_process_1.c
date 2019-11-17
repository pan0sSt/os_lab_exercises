#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
  sem_t *sem;
  sem = sem_open("mysem1", O_CREAT, 0644, 1);
  int i;
  if (fork())
  {
    for (i=0;i<10;i++){
      sem_wait(sem);
      display("Hello world\n");
    sem_post(sem);}
    wait(NULL);
  }
  else
  {
    for (i=0;i<10;i++){
      sem_wait(sem);
      display("Kalimera kosme\n");
    sem_post(sem);}
  }
  sem_close(sem);
  sem_unlink("mysem1");
  sem_destroy(sem);
  return 0;
}

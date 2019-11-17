#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
  int i;
  sem_t *sem;
  sem_t *sem1;

  sem = sem_open("mysem0", O_CREAT, 0644, 1);
  sem1 = sem_open("mysem1", O_CREAT, 0644, 0);

  if (fork())
  {
    for (i=0;i<10;i++){
      sem_wait(sem);
      display("ab");
      sem_post(sem1);
    }
    wait(NULL);
  }
  else
  {
    for (i=0;i<10;i++){
      sem_wait(sem1);
      display("cd\n");
      sem_post(sem);
    }
  }
  sem_close(sem);
  sem_unlink("mysem0");
  sem_destroy(sem);
  sem_close(sem1);
  sem_unlink("mysem1");
  sem_destroy(sem1);
  return 0;
}

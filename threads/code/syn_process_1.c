#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"

int main()
{
  int i;

  if (fork())
  {
    for (i=0;i<10;i++)
      display("Hello world\n");
    wait(NULL);
  }
  else
  {
    for (i=0;i<10;i++)
      display("Kalimera kosme\n");
  }

  return 0;
}

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int p[2];
  pipe(p);

  if (fork() == 0) {
    char buf[2];
    read(p[0], buf, 1);
    printf("%d: received ping\n", getpid());
    close(p[0]);

    write(p[1], "b", 1);
    close(p[1]);

    exit(0);
  } else {
    write(p[1], "a", 1);
    close(p[1]);

    wait(0);
    
    char buf[2];
    read(p[0], buf, 1);
    printf("%d: received pong\n", getpid());
    close(p[0]);
  }
  exit(0);
}
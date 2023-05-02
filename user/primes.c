#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void child_process(int p[2]) {
  close(p[1]);

  int i;
  // no data to read
  if (read(p[0], &i, sizeof(i)) == 0) {
    close(p[0]);
    exit(0);
  }
  
  printf("prime %d\n", i);

  int num;
  int p_p2c[2];
  pipe(p_p2c);

  if (fork() == 0) {
    child_process(p_p2c);
  } else {
    close(p_p2c[0]);
    while (read(p[0], &num, sizeof(num)) > 0) {
      if (num % i != 0) {
        write(p_p2c[1], &num, sizeof(num));
      }
    }
    close(p_p2c[1]);
    wait(0);
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  int p[2];

  pipe(p);

  if (fork() == 0) {
    child_process(p);
  } else {
    close(p[0]);
    for (int i = 2; i < 35; i ++ ) {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
    wait(0);
  }

  exit(0);
}

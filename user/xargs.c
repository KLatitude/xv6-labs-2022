#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char buf[512];
  char *arg[100];

  if (argc < 2) {
    printf("xargs: less arg\n");
    exit(0);
  } 

  for (int i = 1; i < argc; i ++ )
    arg[i - 1] = argv[i];
  while (1) {
    char *a = buf;
    char b;

    while (read(0, &b, sizeof(b)) > 0 && b != '\n') {
      *a = b;
      a ++ ;
    }
    *a = 0;
    arg[argc - 1] = buf;
    if (fork() == 0) {
      exec(argv[1], arg);
      exit(0);
    } else {
      wait(0);
    }

    if (read(0, &b, sizeof(b)) == 0)
      break;
  }
  exit(0);
}

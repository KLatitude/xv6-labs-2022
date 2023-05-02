#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// get filename from path
char* fmtname(char *path) {
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash
  for (p = path + strlen(path); p >= path && *p != '/'; p -- )
    ;
  p ++ ;

  memmove(buf, p, strlen(p) + 1);
  return buf;
}

void check_print(char *filename, char *findname) {
  if (strcmp(fmtname(filename), findname) == 0)
    printf("%s\n", filename);
}

void find(char *path, char *findname) {
  char buf[512], *p;
  int fd;
  struct dirent de; // refer to directory entry
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
  //printf("path: %s st_type: %d\n", path, st.type);
  switch (st.type) {
    case T_FILE:
      check_print(path, findname);
      break;

    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        
        find(buf, findname);
      }
      break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("find: find <path> <filename>\n");
    exit(0);
  }

  find(argv[1], argv[2]);
  exit(0);
}

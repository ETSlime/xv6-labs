#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return non blank pad name.
  // you need to add 1 to the length of p
  // because strlen doens't count \0
  memmove(buf, p, strlen(p) + 1);
//  printf("%s\n", buf);
//  printf("%d\n", strlen(buf));
  return buf;

  // no blank pad
 
//  if(strlen(p) >= DIRSIZ)
//    return p;
//  memmove(buf, p, strlen(p));
//  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
//  return buf;
}

void find(char *path, char *name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if (strcmp(fmtname(path), name) == 0)
	    printf("%s\n", path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0; // set end
      // don't recurse into "." and ".."
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      {
		continue;
      }
      // recursive find
      find(buf, name);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if(argc < 3){
    printf("ERROR\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}

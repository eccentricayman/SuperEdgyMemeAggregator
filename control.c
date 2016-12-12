#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>

void display() {
  struct stat stats;
  stat("telephone", &stats);
  int size = stats.st_size;
  int f = open("telephone", O_RDONLY);
  char b[size];
  read(f, b, size);
  int i;
  for (i = 0; i < size; i++)
    printf("%c", b[i]);
  printf("\n");
  close(f);
}

int main(int argc, char * argv[]) {

  int sd;
  int semid;
  int sc;
  int key = ftok("makefile", 22);

  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0) {
    sd = shmget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
    semctl(semid, 0, SETVAL, 1);
    int f = open("telephone", O_CREAT | O_TRUNC, 0644);
  }
  else if (strncmp(argv[1], "-r", strlen(argv[1])) == 0) {
    sd = shmget(key, sizeof(int), IPC_CREAT | 0644);
    semid = semget(key, 1, 0);
    shmctl(sd, IPC_RMID, 0);
    semctl(semid, 0, IPC_RMID);
    display();
  }
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0) {
    display();
  }
  
  return 0;
}

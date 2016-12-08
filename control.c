#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <fcntl.h>

int main(int argc, char * argv[]) {

  int sd;
  int sem;
  int sc;
  int key = ftok("makefile", 22);

  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0) {
    sd = shmget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
    sem = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);
  }

  else if (strncmp(argv[1], "-r", strlen(argv[1])) == 0) {
    sd = shmget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
    
    sem = semget(key, 1, 0);
    sc = semctl(sem, 0, IPC_RMID);
  }

  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0) {

  }
  
  return 0;
}

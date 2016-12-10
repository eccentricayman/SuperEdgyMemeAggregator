#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>

/*****************************************************************/
/* Will attempt to access the resources using the semaphore      */
/* Once in, it should display the last line added to the file    */
/* (the shared memory should contain the size of the last line). */
/* Then prompt the use for the next line.                        */
/* Once a new line is added, it should write that to the file,   */
/* update the shared memory and then release the semaphore       */
/*****************************************************************/

int main() {
    
    int fd = open("telephone", O_EXCL | O_RDWR | O_APPEND, 0644);
    int semID = semget(ftok("makefile", 22), 1, 0);

    int shmID = shmget(ftok("makefile", 22), sizeof(int), IPC_EXCL);

    int * shmVal;
    shmat(shmID, shmVal, 0);
    
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;

    semop(semID, &sb, 1);

    lseek(fd, -*shmVal, SEEK_END);
    char lastLine[*shmVal];
    read(fd, lastLine, *shmVal);
    printf("Current Last Line: %s\n", lastLine);
    lseek(fd, 0, SEEK_END);

    char input[26144];
    printf("Add a new line: ");
    fgets(input, 26144, stdin);
    
    char trimmedInput[strlen(input)];
    strcpy(trimmedInput, input);

    //shmVal = sizeof(trimmedInput);
    //setval sharedmem

    write(fd, trimmedInput, strlen(trimmedInput));
    close(fd);

    *shmVal = (int)sizeof(trimmedInput);

    sb.sem_op = 1;
    semop(semID, &sb, 1);
    
}

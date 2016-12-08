#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>

/****************************************************************************************************************************/
/* Will attempt to access the resources using the semaphore                                                                 */
/* Once in, it should display the last line added to the file (the shared memory should contain the size of the last line). */
/* Then prompt the use for the next line.                                                                                   */
/* Once a new line is added, it should write that to the file, update the shared memory and then release the semaphore      */
/****************************************************************************************************************************/

int main() {
    
    int fd = open("telephone.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
    
    int semID = semget(ftok("makefile", 22), 1, 0);
    int semVal = semctl(semID, 0, GETVAL);
    union semun semunion;

    lseek(fd, -semVal, SEEK_END);
    char lastLine[semVal];
    read(fd, lastLine, semVal);
    printf("Current Last Line: %s\n", lastLine);
    lseek(fd, 0, SEEK_END);
    
    char input[26144];
    
    printf("Add a new line: ");
    fgets(input, 26144, STDIN_FILENO);
    
    char trimmedInput[strlen(input)];
    strcpy(trimmedInput, input);

    semunion.val = sizeof(trimmedInput);
    semctl(semID, 0, SETVAL, semunion);

    write(fd, trimmedInput, strlen(trimmedInput));
    close(fd);

}

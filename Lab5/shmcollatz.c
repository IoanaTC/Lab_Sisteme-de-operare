#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

#define SHM_SIZE 1024

int main(int argc, char *argv[]){

    char *shm_name = "sharedmem_file";

    // create shared mem file
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(shm_fd < 0){
        perror("Open shm error");
        return errno;
    }

    // def dimensiune
    if(ftruncate(shm_fd, SHM_SIZE) == -1){
        perror("Truncate error");
        shm_unlink(shm_name);
        return errno;
    }
    // create pointer to shm object
    void *shm_ptr_child = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shm_ptr_child == MAP_FAILED){
        perror("MMAP failed");
        shm_unlink(shm_name);
        return errno;
    }
    // este nevoie de un offset pentru a nu suprascrie datele in memoria partajata
    int offset = 200;
    pid_t pid;

    for(int i=1; i<argc; i++){
        pid_t pid = fork();
        int number = atoi(argv[i]);

        if(pid < 0){
            perror("Fork error");
            return errno;
        }
        else if(pid == 0){
            // ramura copil
            // index = offset pentru fiecare nr scris pentru parametrul curent
            int index = 0;

            sprintf(shm_ptr_child  + offset * (i-1) + index, "%d: ", number);
            index+=5;

            while(number > 1){
                if(number % 2 == 0){
                    number = number / 2;
                    sprintf(shm_ptr_child  + offset * (i-1) + index, "%d ", number);
                    index+=5;
                }
                else{
                    number = number*3 + 1;
                    sprintf(shm_ptr_child + offset * (i-1) + index, "%d ", number);
                    index+=5;
                }
            }
            sprintf(shm_ptr_child + offset * (i-1) + index, "\n");     
            return 0;
        }
    }
    for(int i=1; i<argc; i++){
        pid_t child = wait(NULL);
        printf("Done Parent %d Me %d\n", getpid(), child);
    }
    // eliminam pointerul catre obiectul shm
    munmap(shm_ptr_child, SHM_SIZE);

    pid = fork();

    if(pid == 0){
        // afisam continutul fisierului shm 
        wait(NULL);
        char * targv[] = {"cat", "/dev/shm/sharedmem_file", NULL};
        if(execve("/usr/bin/cat", targv, NULL) < 0){
            perror("Execve error");
            return errno;
        }
    }
    else if(pid > 0){
        // eliminam fisierul shm 
        wait(NULL);

        char * targv[] = {"rm", "/dev/shm/sharedmem_file", NULL};
        if(execve("/usr/bin/rm", targv, NULL) < 0){
            perror("Execve error");
            return errno;
        }
    }
    else{
        perror("Fork error");
        return errno;
    }
}
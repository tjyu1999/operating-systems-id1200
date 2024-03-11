# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <semaphore.h>
# include <sys/shm.h>
# include <sys/wait.h>

# define MAX 5

sem_t *semA;
sem_t *semB;

void* reader_process(int *var){
    while(1){
        if(*var >= MAX){
            exit(0);
            break;
        }
        
        int val;
        sem_wait(semA);
        sem_getvalue(semB, &val);
        
        if(val == 1){
            sem_wait(semB);
            printf("The first reader aquires the lock.\n");
        }
        printf("The reader (%d) reads the value %d\n", getpid(), *var);
        
        if(val == 0){
            sem_post(semB);
            printf("The last reader releases the lock.\n\n");
        }
        sem_post(semA);
        sleep(2);
    }
}

void* writer_process(int* var){
    while(1){
        sem_wait(semA);
        printf("The writer aquires the lock.\n");
        printf("The writer (%d) writes the value %d\n", getpid(), ++*var);
        printf("The writer releases the lock.\n\n");
        sem_post(semA);        
        
        if(*var >= MAX) break;
        sleep(2);
    }
}

int main(){
    int shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int *var = shmat(shm_id, NULL, 0);
    *var = 0;
    
    semA = sem_open("/semA", O_CREAT | O_RDWR, 0600, 1);
    semB = sem_open("/semB", O_CREAT | O_RDWR, 0600, 1);
    
    pid_t reader1 = fork();
    if(reader1 == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(reader1 == 0) reader_process(var);
    
    pid_t reader2 = fork();
    if(reader2 == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(reader2 == 0) reader_process(var);
    
    pid_t writer = fork();
    if(writer == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(writer == 0) writer_process(var);
    
    wait(NULL);
    sem_close(semA);
    sem_close(semB);
    shmctl(shm_id, IPC_RMID, NULL);
    
    return 0;
}

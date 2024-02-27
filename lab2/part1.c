# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

# define NUM_THREAD  3
# define MAX_BUFFER 15

int buffer = 0;
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

void* thread_function(){
    int cnt = 0;
    pid_t pid = getpid();
    pthread_t tid = pthread_self(); 
    
    while(1){
        if(pthread_mutex_lock(&mutex_lock)){
            perror("pthread_mutex_lock");
            pthread_exit(NULL);
        }
        
        if(buffer == MAX_BUFFER){
            if(pthread_mutex_unlock(&mutex_lock)){
                perror("pthread_mutex_unlock");
                pthread_exit(NULL);
            }
         
            break;
        }
        
        printf("TID: %u, PID: %d, Buffer: %d\n", *((int*) tid), pid, ++buffer);
        cnt++;
        
        if(pthread_mutex_unlock(&mutex_lock)){
            perror("pthread_mutex_unlock");
            pthread_exit(NULL);
        }
        
        usleep(rand() % 100);
    }
    
    printf("TID %u worked on the buffer %d times\n", *((int*) tid), cnt);
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NUM_THREAD];
    
    for(int i = 0; i < NUM_THREAD; i++){
        if(pthread_create(&threads[i], NULL, thread_function, NULL)){
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    
    for(int j = 0; j < NUM_THREAD; j++) pthread_join(threads[j], NULL);
    printf("Total buffer accesses: %d\n", buffer);
    pthread_mutex_destroy(&mutex_lock);
    
    return 0;
}

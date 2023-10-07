# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int num_thread = 3;
int buffer = 0;
int max_buffer = 15;

void* thread_function(void* thread_id){
    int count = 0;
    
    while(1){    
        int mutex_lock = pthread_mutex_lock(&mutex);
        if(mutex_lock){
            perror("pthread_mutex_lock");
            pthread_exit(NULL);
        }
        
        if(buffer == max_buffer){
            int mutex_unlock = pthread_mutex_unlock(&mutex);
            if(mutex_unlock){
                perror("pthread_mutex_unlock");
                pthread_exit(NULL);
            }
         
            break;
        }
        
        printf("TID: %u, PID: %d, Buffer: %d\n", *((int*) thread_id), getpid(), buffer);    
        buffer++;
        count++;
        
        int mutex_unlock = pthread_mutex_unlock(&mutex);
        if(mutex_unlock){
            perror("pthread_mutex_unlock");
            pthread_exit(NULL);
        }
        
        usleep(rand() % 1000);
    }
    
    pthread_exit((void*)(intptr_t) count);
}

int main(){
    int access_time = 0;
    pthread_t thread_id[num_thread];
    pthread_mutex_init(&mutex, NULL);
    
    for(int i = 0; i < num_thread; i++){
        int result = pthread_create(&thread_id[i], NULL, thread_function, &thread_id[i]);
        if(result){
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    
    for(int j = 0; j < num_thread; j++){
        void* thread_result;
        int thread_count;
        
        pthread_join(thread_id[j], &thread_result);
        thread_count = (intptr_t) thread_result;
        access_time += thread_count;
        
        printf("TID %u worked on the buffer %d times\n", *((int*) thread_id[j]), thread_count);
    }    
    printf("Total buffer accesses: %d\n", access_time);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}

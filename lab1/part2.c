#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure of the message queue
struct message_queue{
    long msg_type;
    char msg_text[100];
};

int main() {
    // create a message queue
    key_t key = ftok("/tmp", 'A');
    int msg_id = msgget(key, 0666 | IPC_CREAT);

    // create the child process
    pid_t child_pid = fork();
    
    // the child process
    if (child_pid == 0){
        struct message_queue buffer;
        
        // receive the message
        msgrcv(msg_id, &buffer, sizeof(buffer), 1, 0);

        // count the number of words in the message
        int count = 0;
        char* token = strtok(buffer.msg_text, " ");
        while(token != NULL){
            count++;
            token = strtok(NULL, " ");
        }
        
        printf("%d\n", count);

        // remove the message queue
        msgctl(msg_id, IPC_RMID, NULL);
    }
    
    // the parent process
    else{
        struct message_queue buffer;

        // read the content of the message
        FILE *file = fopen("test.txt", "r");

        while(fgets(buffer.msg_text, sizeof(buffer.msg_text), file) != NULL) {
            buffer.msg_type = 1;
            msgsnd(msg_id, &buffer, sizeof(buffer), 0);
        }

        fclose(file);
    }
    
    return 0;
}

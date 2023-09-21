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

int main(){
    // generate an unique IPC and create a message queue
    key_t key = ftok("/tmp", 'A');
    int msg_id = msgget(key, 0666 | IPC_CREAT);

    // create the child process
    pid_t child_pid = fork();
    
    // the child process
    if(child_pid == 0){
        struct message_queue msq;
        
        // receive the message
        msgrcv(msg_id, &msq, sizeof(msq), 1, 0); // ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
                                                 // int msqid: id of the message queue 
                                                 // void *msgp: pointer to where the received message will be stored
                                                 // size_t msgsz: size of the message queue
                                                 // long msgtyp: type of the received mes
                                                 // int msgflg: a set of flags used to control the behavior of the function

        // count the number of words in the message
        int count = 0;
        char *token = strtok(msq.msg_text, " ");
        while(token != NULL){
            count++;
            token = strtok(NULL, " ");
        }
        
        printf("%d\n", count);

        // remove the message queue
        msgctl(msg_id, IPC_RMID, NULL); // int msgctl(int msqid, int cmd, struct msqid_ds *buf);
                                        // int msqid: id of the message queue
                                        // int cmd: control command that specifies what operation the function should perform
                                        // struct msqid_ds *buf: pointer used for setting or obtaining attributes of the message queue
    }
    
    // the parent process
    else{
        struct message_queue msq;

        // read the content of the message
        FILE *file = fopen("test.txt", "r");

        while(fgets(msq.msg_text, sizeof(msq.msg_text), file) != NULL){
            msq.msg_type = 1;
            msgsnd(msg_id, &msq, sizeof(msq), 0); // int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
                                                  // int msqid: id of the message queue
                                                  // const void *msgp: pointer to where the message will be sent
                                                  // size_t: size of the message queue
                                                  // int msgflg: a set of flags used to control the behavior of the function
        }

        fclose(file);
    }
    
    return 0;
}

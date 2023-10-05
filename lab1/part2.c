# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

// structure of the message queue
struct message_queue{
    long msg_type;
    char msg_text[100];
};

int main(){
    key_t key;    
    // generate an unique IPC and check the exception
    if((key = ftok("/tmp", 'A')) == -1){
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    // create a message queue and check the exception
    int msq_id;
    if((msq_id = msgget(key, 0666 | IPC_CREAT)) == -1){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // create the child process and check the exception
    pid_t child_pid = fork();
    if(child_pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    // the child process
    if(child_pid == 0){
        struct message_queue msq;
        
        // receive the message and check the exception
        /* ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
               int msqid: id of the message queue 
               void *msgp: pointer to where the received message will be stored
               size_t msgsz: size of the message queue
               long msgtyp: type of the received mes
               int msgflg: a set of flags used to control the behavior of the function */
        if(msgrcv(msq_id, &msq, sizeof(msq), 1, 0) == -1){
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // count the number of words in the message
        int count = 0;
        char *token = strtok(msq.msg_text, " ");
        while(token != NULL){
            count++;
            token = strtok(NULL, " ");
        }
        
        printf("%d\n", count);
    }
    
    // the parent process
    else{
        struct message_queue msq;

        // read the content of the message and check the exception
        FILE *file = fopen("test.txt", "r");
        if(file == NULL){
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        while(fgets(msq.msg_text, sizeof(msq.msg_text), file) != NULL){
            msq.msg_type = 1;
            /* int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
                   int msqid: id of the message queue
                   const void *msgp: pointer to where the message will be sent
                   size_t: size of the message queue
                   int msgflg: a set of flags used to control the behavior of the function */
            if(msgsnd(msq_id, &msq, sizeof(msq.msg_text), 0) == -1){
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }

        fclose(file);
    }
    
    // remove the message queue
    msgctl(msq_id, IPC_RMID, NULL); /* int msgctl(int msqid, int cmd, struct msqid_ds *buf);
                                           int msqid: id of the message queue
                                           int cmd: control command that specifies what operation the function should perform
                                           struct msqid_ds *buf: pointer used for setting or obtaining attributes of the message queue */
    
    return 0;
}

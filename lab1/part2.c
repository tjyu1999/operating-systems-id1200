# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

struct message_queue{
    long msg_type;
    char msg_text[100];
};

int main(){
    key_t key;    
    if((key = ftok("/tmp", 'A')) == -1){
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    
    int msq_id;
    if((msq_id = msgget(key, 0666 | IPC_CREAT)) == -1){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if(pid == 0){
        struct message_queue msq;
        
        if(msgrcv(msq_id, &msq, sizeof(msq), 1, 0) == -1){
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        int cnt = 0;
        char* token = strtok(msq.msg_text, " ");
        while(token != NULL){
            cnt++;
            token = strtok(NULL, " ");
        }
        
        printf("%d\n", cnt);
    }
    
    else{
        struct message_queue msq;

        FILE* file = fopen("text.txt", "r");
        if(file == NULL){
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        while(fgets(msq.msg_text, sizeof(msq.msg_text), file) != NULL){
            msq.msg_type = 1;
            
            if(msgsnd(msq_id, &msq, sizeof(msq.msg_text), 0) == -1){
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }

        fclose(file);
    }
    
    msgctl(msq_id, IPC_RMID, NULL);
    return 0;
}

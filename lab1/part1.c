# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>

int main(){
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();
    if(pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	execlp("/bin/ls", "ls", "/", NULL);
    }

    else{
        close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	execlp("/bin/wc", "wc", "-l", NULL);
    }

    wait(NULL);
    return 0;
}

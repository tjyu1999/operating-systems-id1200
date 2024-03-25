# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

int main(){
    int pipefd[2];
    if(pipe(pipefd) == -1) return 1;
    
    pid_t pid;
    pid = fork(); /* fork a child process */
    
    /* error occurred */
    if(pid < 0) return 1;
    
    /* child process */
    else if(pid == 0){
        close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	execlp("/bin/ls", "ls", "/", NULL);
    }
    
    /* parent process */
    else{
        close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	execlp("/bin/wc", "wc", "-l", NULL);
    }

    wait(NULL);
    return 0;
}

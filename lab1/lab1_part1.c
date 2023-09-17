# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>

int main(){
    int pipe_mod[2];

    // create the pipe
    if(pipe(pipe_mod) == -1){
        perror("pipe");
	exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork();  // create the child process

    if(child_pid == -1){
        perror("fork");
	exit(EXIT_FAILURE);
    }

    if(child_pid == 0){
        close(pipe_mod[0]);
	dup2(pipe_mod[2], STDOUT_FILENO);
	close(pipe_mod[2]);
	execlp("ls", "ls", "/", NULL);
	perror("execlp");
	exit(EXIT_FAILURE);
    }
    else{
        close(pipe_mod[1]);
	dup2(pipe_mod[0], STDIN_FILENO);
	close(pipe_mod[0]);
	execlp("wc", "wc", "-l", NULL);
	perror("execlp");
        exit(EXIT_FAILURE);
    }
    return 0;
}

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>

int main(){
    int arr[2]; // 0 for reading; 1 for writing

    // create the pipe
    if(pipe(arr) == -1){
        perror("pipe");
	exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork();  // create the child process
    
    // check the return of the fork()
    if(child_pid == -1){
        perror("fork");
	exit(EXIT_FAILURE);
    }

    // the child process
    if(child_pid == 0){
        close(arr[0]); // close the read end of the pipe
	dup2(arr[1], STDOUT_FILENO);
	close(arr[1]); // close the write end of the pipe
	execlp("ls", "ls", "/", NULL); // execute the "ls /" command
	perror("execlp");
	exit(EXIT_FAILURE);
    }

    // the parent process
    else{
        close(arr[1]); //close the write end of the pipe
	dup2(arr[0], STDIN_FILENO);
	close(arr[0]); // close the read end of the pipe
	execlp("wc", "wc", "-l", NULL);
	perror("execlp");
        exit(EXIT_FAILURE);
    }

    return 0;
}

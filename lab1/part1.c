# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>

int main(){
    // 0 for reading; 1 for writing
    int arr[2];

    // create the pipe
    if(pipe(arr) == -1){
        perror("pipe");
	exit(EXIT_FAILURE);
    }
    
    // create the child process
    pid_t child_pid = fork();

    // the child process
    if(child_pid == 0){
        close(arr[0]);
	dup2(arr[1], STDOUT_FILENO);
	close(arr[1]);
	
	// execute the "ls /" command
	execlp("ls", "ls", "/", NULL);
    }

    // the parent process
    else{
        close(arr[1]);
	dup2(arr[0], STDIN_FILENO);
	close(arr[0]);
	
	// execute the "wc -l" command
	execlp("wc", "wc", "-l", NULL);
    }

    return 0;
}

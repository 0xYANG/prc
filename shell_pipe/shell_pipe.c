#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char* argv[])
{

    int pipe_fd[2];
    int ret_code = 0;

    ret_code = pipe(pipe_fd);
    if (ret_code < 0) {
        perror("[ERROR]: pipe() failed!");
        exit(EXIT_FAILURE);
    }

    int pid = fork();
    if (pid == 0) {   // child 
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        ret_code = execlp(argv[1], argv[1], NULL);
        if (ret_code < 0) {
            perror("[ERROR]: execlp() failed!");
            exit(EXIT_FAILURE);
        }
    } else if (pid > 0) {  // parent
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        ret_code = execlp(argv[2], argv[2], NULL);
        if (ret_code < 0) {
            perror("[ERROR]: execlp() failed!");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("[ERROR]: fork() failed!");
        exit(EXIT_FAILURE);
    }

    return 0;
}

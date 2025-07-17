#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    // Create a child process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // This is the child process
        printf("Child process: PID = %d\n", getpid());

        // Exit with a specific exit code
        exit(12);
    } else {
        // This is the parent process
        printf("Parent process: Waiting for child to exit...\n");

        // Wait for the child process to terminate
        wait(&status);

        // Check if the child exited normally
        if (WIFEXITED(status)) {
            // Retrieve and print the exit status of the child
            printf("Parent process: Child exited with status = %d\n", WEXITSTATUS(status));
        } else {
            // The child did not terminate normally
            printf("Parent process: Child did not exit normally.\n");
        }
    }

    return 0;
}

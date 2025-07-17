#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

// Signal handler for SIGUSR1
void handle_sigusr1(int sig) {
    printf("Child process received SIGUSR1 signal (signal number: %d)\n", sig);
    fflush(stdout);  // Ensure output is immediately flushed
}

int main() {
    pid_t pid;

    // Fork a new process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child process started. PID: %d\n", getpid());

        // Register signal handler for SIGUSR1
        signal(SIGUSR1, handle_sigusr1);

        // Wait for signal from parent
        while (1) {
            pause();  // Suspend execution until a signal is received
        }
    } else {
        // Parent process
        printf("Parent process started. PID: %d\n", getpid());
        sleep(2);  // Delay before sending signal

        // Send SIGUSR1 to child process
        kill(pid, SIGUSR1);
        printf("Parent sent SIGUSR1 to child process (PID: %d)\n", pid);

        // Optional: Wait before exiting
        sleep(1);
    }

    return 0;
}

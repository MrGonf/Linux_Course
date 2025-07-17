#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Child process
        printf("Child process started. PID: %d, PPID: %d\n", getpid(), getppid());

        // Sleep to simulate long-running child (for orphan case)
        sleep(10);

        printf("Child process exiting. PID: %d, New PPID: %d\n", getpid(), getppid());
        exit(0);
    } else {
        // Parent process
        printf("Parent process started. PID: %d\n", getpid());

        // Sleep to allow observation of zombie (child will exit before parent reaps)
        sleep(3); // After child exits but before parent calls wait

        printf("Parent process exiting without waiting for child. PID: %d\n", getpid());
        // Not calling wait(): this creates zombie process temporarily
        exit(0); // Parent exits before child finishes (child becomes orphan)
    }
}

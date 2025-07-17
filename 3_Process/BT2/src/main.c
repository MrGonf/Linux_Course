#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * This program demonstrates how a child process can replace itself
 * with another program using execlp(), based on an environment variable MODE.
 *
 * MODE=1 --> run "ls -l"
 * MODE=2 --> run "date"
 */

int main() {
    pid_t pid;
    char *mode_str = getenv("MODE");

    if (mode_str == NULL) {
        fprintf(stderr, "Error: MODE environment variable not set.\n");
        return 1;
    }

    int mode = atoi(mode_str);

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) executing...\n", getpid());

        if (mode == 1) {
            // Replace with 'ls -l'
            execlp("ls", "ls", "-l", NULL);
        } else if (mode == 2) {
            // Replace with 'date'
            execlp("date", "date", NULL);
        } else {
            fprintf(stderr, "Invalid MODE value. Use 1 for ls or 2 for date.\n");
            exit(1);
        }

        // If exec fails
        perror("execlp failed");
        exit(1);
    } else {
        // Parent process
        wait(NULL);
        printf("Parent process (PID: %d) finished waiting.\n", getpid());
    }

    return 0;
}

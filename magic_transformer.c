#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int dest_agent_performance = -1;
int dest_state_performance = -1;
int dest_agent_rating = -1;
int dest_state_rating = -1;

void parse_args(int argc, char *argv[]) {
    // Error handling
    if (argc == 1) {
        printf("No output requested, have a nice day!\n");
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        char arg_copy[100];
        strcpy(arg_copy, argv[i]);

        char *func = strtok(arg_copy, ":");
        char *stream = strtok(NULL, ":");

        if (!func || !stream) {
            continue;
        }

        int target_fd = -1;
        if (strcmp(stream, "stdout") == 0) {
            target_fd = STDOUT_FILENO;
        } else if (strcmp(stream, "stderr") == 0) {
            target_fd = STDERR_FILENO;
        } else {
            continue;
        }

        // Error handling
        if (strcmp(func, "agent_performance") == 0) {
            if (dest_agent_performance != -1 && dest_agent_performance != target_fd) {
                fprintf(stderr, "Error: agent_performance is directed to more than one output stream!\n");
                exit(1);
            }
            dest_agent_performance = target_fd;
        } else if (strcmp(func, "state_performance") == 0) {
            if (dest_state_performance != -1 && dest_state_performance != target_fd) {
                fprintf(stderr, "Error: state_performance is directed to more than one output stream!\n");
                exit(1);
            }
            dest_state_performance = target_fd;
        } else if (strcmp(func, "agent_rating") == 0) {
            if (dest_agent_rating != -1 && dest_agent_rating != target_fd) {
                fprintf(stderr, "Error: agent_rating is directed to more than one output stream!\n");
                exit(1);
            }
            dest_agent_rating = target_fd;
        } else if (strcmp(func, "state_rating") == 0) {
            if (dest_state_rating != -1 && dest_state_rating != target_fd) {
                fprintf(stderr, "Error: state_rating is directed to more than one output stream!\n");
                exit(1);
            }
            dest_state_rating = target_fd;
        }
    }
}

int main(int argc, char *argv[]) {
    parse_args(argc, argv);

    // Create pipes
    int pipe1[2];
    int pipe2[2];

    // Error handling
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    int null_fd = open("/dev/null", O_WRONLY);

    // Transformer 1
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Close
        close(pipe1[0]);
        close(pipe2[0]);

        // Redirect stdout
        dup2(pipe1[1], STDOUT_FILENO);
        
        // Redirect stderr
        dup2(pipe2[1], STDERR_FILENO);

        // Close
        close(pipe1[1]);
        close(pipe2[1]);
        close(null_fd);

        execl("./transformer1", "transformer1", NULL);
        perror("Exec T1 failed");
        exit(1);
    }

    // Transformer 2
    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Close
        close(pipe1[1]); 
        close(pipe2[0]); 
        close(pipe2[1]);

        // Redirect
        dup2(pipe1[0], STDIN_FILENO);
        close(pipe1[0]);

        // Stdout
        if (dest_agent_performance != -1) {
            dup2(dest_agent_performance, STDOUT_FILENO);
        } else {
            dup2(null_fd, STDOUT_FILENO);
        }

        // Stderr
        if (dest_state_performance != -1) {
            dup2(dest_state_performance, STDERR_FILENO);
        } else {
            dup2(null_fd, STDERR_FILENO);
        }

        close(null_fd);
        execl("./transformer2", "transformer2", NULL);
        perror("Exec T2 failed");
        exit(1);
    }

    // Transformer 3
    pid_t pid3 = fork();
    if (pid3 == 0) {
        // Close
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        // Redirect
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[0]);

        // Stdout
        if (dest_agent_rating != -1) {
            dup2(dest_agent_rating, STDOUT_FILENO);
        } else {
            dup2(null_fd, STDOUT_FILENO);
        }

        // Stderr
        if (dest_state_rating != -1) {
            dup2(dest_state_rating, STDERR_FILENO);
        } else {
            dup2(null_fd, STDERR_FILENO);
        }

        close(null_fd);
        execl("./transformer3", "transformer3", NULL);
        perror("Exec T3 failed");
        exit(1);
    }

    // Close
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    close(null_fd);

    // Wait
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}

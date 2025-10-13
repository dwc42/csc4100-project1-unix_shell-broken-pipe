#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "wish_exec.h"

// This function is called from within a child process (no fork needed)
void execute_command_child(char *command, char **args, char **paths, char *output_file) {
    if (command == NULL) {
        exit(1);
    }

    // Handle redirection
    if (output_file != NULL) {
        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);
    }

    char exec_path[256];
    int found = 0;

    for (int i = 0; paths != NULL && paths[i] != NULL; i++) {
        snprintf(exec_path, sizeof(exec_path), "%s/%s", paths[i], command);
        if (access(exec_path, X_OK) == 0) {
            found = 1;

            // Build argv for execv()
            char *exec_args[50];
            int j = 0;
            exec_args[j++] = command;
            if (args != NULL) {
                for (int k = 0; args[k] != NULL && j < 49; k++) {
                    exec_args[j++] = args[k];
                }
            }
            exec_args[j] = NULL;

            execv(exec_path, exec_args);

            // If execv returns, it failed
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
    }

    if (!found) {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
}

void execute_command(char *command, char **args, char **paths, char *output_file) {
    if (command == NULL)
        return;

    pid_t pid = fork();
    if (pid < 0) {
        char error_message[30] = "An error has occurred\n";
        write(STDERR_FILENO, error_message, strlen(error_message));
        return;
    }

    if (pid == 0) {

        if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0){
                char error_message[30] = "An error has occurred\n";
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);

            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        // --- Child process ---
        char exec_path[256];
        int found = 0;

        for (int i = 0; paths[i] != NULL; i++) {
            snprintf(exec_path, sizeof(exec_path), "%s/%s", paths[i], command);
            if (access(exec_path, X_OK) == 0) {
                found = 1;

                // Build argv for execv()
                char *exec_args[50];
                int j = 0;
                exec_args[j++] = command;
                if (args != NULL) {
                    for (int k = 0; args[k] != NULL && j < 49; k++) {
                        exec_args[j++] = args[k];
                    }
                }
                exec_args[j] = NULL;

                execv(exec_path, exec_args);

                char error_message[30] = "An error has occurred\n";
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
        }

        if (!found) {
            char error_message[30] = "An error has occurred\n";
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
    } else {
        // --- Parent process waits for child ---
        int status;
        waitpid(pid, &status, 0);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_PATHS 64
#define MAX_CMDS 64
#define MAX_ARGS 64

char error_message[30] = "An error has occurred\n";
char *path[MAX_PATHS];
int path_count = 1;

/* Function declarations */
void print_error();
void init_path();
void free_path();
char *find_executable(char *cmd);
int handle_builtin(char **args, int arg_count);
void free_args(char **args, int count);
int parse_command(char *cmd, char ***args_out, char **redirect_out);
void process_line(char *line);

void print_error() {
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void init_path() {
    path[0] = strdup("/bin");
}

void free_path() {
    for (int i = 0; i < path_count; i++) {
        free(path[i]);
    }
    path_count = 0;
}

char *find_executable(char *cmd) {
    if (path_count == 0) return NULL;
    
    for (int i = 0; i < path_count; i++) {
        char *full_path = malloc(strlen(path[i]) + strlen(cmd) + 2);
        sprintf(full_path, "%s/%s", path[i], cmd);
        if (access(full_path, X_OK) == 0) {
            return full_path;
        }
        free(full_path);
    }
    return NULL;
}

int handle_builtin(char **args, int arg_count) {
    if (strcmp(args[0], "exit") == 0) {
        if (arg_count > 1) {
            print_error();
            return 1;
        }
        exit(0);
    }
    
    if (strcmp(args[0], "cd") == 0) {
        if (arg_count != 2) {
            print_error();
            return 1;
        }
        if (chdir(args[1]) != 0) {
            print_error();
        }
        return 1;
    }
    
    if (strcmp(args[0], "path") == 0) {
        free_path();
        for (int i = 1; i < arg_count; i++) {
            path[path_count++] = strdup(args[i]);
        }
        return 1;
    }
    
    return 0;
}

void free_args(char **args, int count) {
    for (int i = 0; i < count; i++) {
        free(args[i]);
    }
    free(args);
}

int parse_command(char *cmd, char ***args_out, char **redirect_out) {
    *redirect_out = NULL;
    
    char *redirect_pos = strchr(cmd, '>');
    if (redirect_pos != NULL) {
        *redirect_pos = '\0';
        redirect_pos++;
        
        char *token;
        char *saveptr;
        int redirect_count = 0;
        char *redirect_file = NULL;
        
        token = strtok_r(redirect_pos, " \t\n", &saveptr);
        while (token != NULL) {
            if (strchr(token, '>') != NULL) {
                print_error();
                return -1;
            }
            redirect_count++;
            if (redirect_count == 1) {
                redirect_file = token;
            }
            token = strtok_r(NULL, " \t\n", &saveptr);
        }
        
        if (redirect_count != 1) {
            print_error();
            return -1;
        }
        
        *redirect_out = strdup(redirect_file);
    }
    
    char **args = malloc(sizeof(char *) * MAX_ARGS);
    int arg_count = 0;
    char *token;
    char *saveptr;
    
    token = strtok_r(cmd, " \t\n", &saveptr);
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count++] = strdup(token);
        token = strtok_r(NULL, " \t\n", &saveptr);
    }
    args[arg_count] = NULL;
    
    *args_out = args;
    return arg_count;
}

void process_line(char *line) {
    char *commands[MAX_CMDS];
    int cmd_count = 0;
    char *saveptr;
    char *token;
    
    token = strtok_r(line, "&", &saveptr);
    while (token != NULL && cmd_count < MAX_CMDS) {
        commands[cmd_count++] = token;
        token = strtok_r(NULL, "&", &saveptr);
    }
    
    pid_t pids[MAX_CMDS];
    int pid_count = 0;
    
    for (int i = 0; i < cmd_count; i++) {
        char **args;
        char *redirect_file;
        int arg_count = parse_command(commands[i], &args, &redirect_file);
        
        if (arg_count < 0) {
            continue;
        }
        
        if (arg_count == 0) {
            free_args(args, 0);
            if (redirect_file) free(redirect_file);
            continue;
        }
        
        if (handle_builtin(args, arg_count)) {
            free_args(args, arg_count);
            if (redirect_file) free(redirect_file);
            continue;
        }
        
        char *exec_path = find_executable(args[0]);
        if (exec_path == NULL) {
            print_error();
            free_args(args, arg_count);
            if (redirect_file) free(redirect_file);
            continue;
        }
        
        pid_t pid = fork();
        if (pid < 0) {
            print_error();
            free(exec_path);
            free_args(args, arg_count);
            if (redirect_file) free(redirect_file);
            continue;
        }
        
        if (pid == 0) {
            if (redirect_file != NULL) {
                int fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    print_error();
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);
            }
            
            execv(exec_path, args);
            print_error();
            exit(1);
        }
        
        pids[pid_count++] = pid;
        free(exec_path);
        free_args(args, arg_count);
        if (redirect_file) free(redirect_file);
    }
    
    for (int i = 0; i < pid_count; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    int interactive = 1;
    
    if (argc > 2) {
        print_error();
        exit(1);
    }
    
    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            print_error();
            exit(1);
        }
        interactive = 0;
    }
    
    init_path();
    
    char *line = NULL;
    size_t len = 0;
    
    while (1) {
        if (interactive) {
            printf("wish> ");
            fflush(stdout);
        }
        
        ssize_t nread = getline(&line, &len, input);
        
        if (nread == -1) {
            break;
        }
        
        process_line(line);
    }
    
    free(line);
    if (input != stdin) {
        fclose(input);
    }
    free_path();
    
    return 0;
}
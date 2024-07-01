#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

#define MAX_ARGS 10
#define MAX_LINE 80

int should_run = 1;

int main(void) {
    char *args[MAX_ARGS][MAX_LINE / 2 + 1];
    int num_commands = 0;

    while (should_run) {
        char *line = readline("it007sh> ");
        if (line == NULL) {
            break;
        } else if (*line) {
            add_history(line);
        }

        int arg_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            if (strcmp(token, "|") == 0) {
                args[num_commands][arg_count] = NULL;
                num_commands++;
                arg_count = 0;
            } else {
                args[num_commands][arg_count++] = token;
            }
            token = strtok(NULL, " ");
        }
        args[num_commands][arg_count] = NULL;
        num_commands++;

        if (strcmp(args[0][0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        int pipes[num_commands - 1][2];
        for (int i = 0; i < num_commands - 1; i++) {
            if (pipe(pipes[i]) == -1) {
                perror("pipe");
                return 1;
            }
        }

        for (int i = 0; i < num_commands; i++) {
            pid_t child_pid = fork();
            if (child_pid == -1) {
                perror("fork");
                return 1;
            } else if (child_pid == 0) {
                // Tiến trình con
                if (i == 0) {
                    // Lệnh đầu tiên
                    if (num_commands > 1) {
                        if (dup2(pipes[0][1], STDOUT_FILENO) == -1) {
                            perror("dup2");
                            return 1;
                        }
                        close(pipes[0][0]);
                        close(pipes[0][1]);
                    }
                } else if (i == num_commands - 1) {
                    // Lệnh cuối cùng
                    if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                        perror("dup2");
                        return 1;
                    }
                    close(pipes[i - 1][0]);
                    close(pipes[i - 1][1]);
                } else {
                    // Lệnh ở giữa
                    if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                        perror("dup2");
                        return 1;
                    }
                    if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                        perror("dup2");
                        return 1;
                    }
                    close(pipes[i - 1][0]);
                    close(pipes[i - 1][1]);
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }

                // Kiểm tra chuyển hướng vào (input redirection)
                char *input_file = NULL;
                for (int j = 0; args[i][j] != NULL; j++) {
                    if (strcmp(args[i][j], "<") == 0) {
                        input_file = args[i][j + 1];
                        args[i][j] = NULL;
                        break;
                    }
                }

                // Kiểm tra chuyển hướng ra (output redirection)
                char *output_file = NULL;
                for (int j = 0; args[i][j] != NULL; j++) {
                    if (strcmp(args[i][j], ">") == 0) {
                        output_file = args[i][j + 1];
                        args[i][j] = NULL;
                        break;
                    }
                }

                // Xử lý chuyển hướng vào
                if (input_file != NULL) {
                    int fd = open(input_file, O_RDONLY);
                    if (fd == -1) {
                        perror("open");
                        exit(1);
                    }
                    if (dup2(fd, STDIN_FILENO) == -1) {
                        perror("dup2");
                        exit(1);
                    }
                    close(fd);
                }

                // Xử lý chuyển hướng ra
                if (output_file != NULL) {
                    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd == -1) {
                        perror("open");
                        exit(1);
                    }
                    if (dup2(fd, STDOUT_FILENO) == -1) {
                        perror("dup2");
                        exit(1);
                    }
                    close(fd);
                }

                execvp(args[i][0], args[i]);
                perror(args[i][0]);
                return 1;
            }
        }

        // Move this loop here
        for (int j = 0; j < num_commands - 1; j++) {
            close(pipes[j][0]);
            close(pipes[j][1]);
        }

        for (int i = 0; i < num_commands; i++) {
            int status;
            wait(&status);
        }

        num_commands = 0;
        free(line);
    }

    return 0;
}

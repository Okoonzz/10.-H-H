#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_LINE 80

int main(void) {
    char *line;
    char *args[MAX_LINE / 2 + 1];
    int should_run = 1;
    int input_redirect = 0, output_redirect = 0;
    char *input_file = NULL, *output_file = NULL;

    using_history();

    while (should_run) {

        //reset lại file
        input_file = NULL;
        output_file = NULL;

        line = readline("it007sh> ");

        if (line && *line) {
            add_history(line);
        }

        if (line == NULL) {
            break;
        }

        line[strcspn(line, "\n")] = '\0'; // Xóa ký tự newline

        int arg_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            if (strcmp(token, "<") == 0) {
                input_redirect = 1;
            } else if (strcmp(token, ">") == 0) {
                output_redirect = 1;
            } else {
                if (input_redirect) {
                    input_file = token;
                    input_redirect = 0;
                } else if (output_redirect) {
                    output_file = token;
                    output_redirect = 0;
                } else {
                    args[arg_count++] = token;
                }
            }
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            continue;
        }

        pid_t child_pid = fork();
        if (child_pid == -1) {
            perror("fork");
            return 1;
        } else if (child_pid == 0) {
            // Tiến trình con

            // Nếu như nhập
            if (input_file != NULL) {
                int fd = open(input_file, O_RDONLY);
                if (fd == -1) {
                    perror(input_file);
                    return 1;
                }
                if (dup2(fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    return 1;
                }
                close(fd);
            }

            //Nếu như xuất
            if (output_file != NULL) {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd == -1) {
                    perror(output_file);
                    return 1;
                }
                if (dup2(fd, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    return 1;
                }
                close(fd);
            }

            execvp(args[0], args);
            perror(args[0]);
            return 1;
        } else {
            // Tiến trình cha
            int status;
            waitpid(child_pid, &status, 0);
        }
    }

    return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LINE 80

int main(void) {
    char line[MAX_LINE];
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;

    while (should_run) {
        printf("it007sh> ");
        fflush(stdout);

        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }

        line[strcspn(line, "\n")] = '\0'; // Xóa ký tự newline

        int arg_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            args[arg_count++] = token;
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
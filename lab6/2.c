#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

#define MAX_LINE 80

int main(void) {
    char *line;
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;

    // Khởi tạo lịch sử lệnh
    using_history();

    while (should_run) {
        // Đọc lệnh từ đầu vào, với khả năng duyệt lịch sử lệnh bằng phím lên/xuống
        line = readline("it007sh> ");

        // Thêm lệnh vào lịch sử nếu không rỗng
        if (line && *line) {
            add_history(line);
        }

        if (line == NULL) {
            break;
        }

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

        free(line);
    }

    return 0;
} 
#include "shell.h"

void free_commands(t_command* command) {
    int i;
    while (command != NULL) {
        t_command* next = command->next_command;
        i = 0;
        if (command->args != NULL) {
            while (command->args[i] != NULL) {
                free(command->args[i]);
                i++;
            }
            free(command->args);
        }
        free(command);
        command = next;
    }
}

char* strip_leading_newline(char* s) {
    int size = strlength(s);
    if (size > 0 && s[size-1] == '\n') {
        s[size-1] = 0;
    }
    return s;
}

int loop_shell(void) {
    t_command* first_command;
    char* line;
    while (1) {
        line = strip_leading_newline(get_next_line(STDIN_FILENO));
        if (line == NULL) break;
        first_command = ming_parse(ming_tokenizer(line));
        int status = ming_launch(first_command);
        if (status == EXIT_MING_CODE) break;
        if (status != 0) {
            printf("Status of last task: %d", status);
        }

        free(line);
        free_commands(first_command);
    }
    return 0;
}
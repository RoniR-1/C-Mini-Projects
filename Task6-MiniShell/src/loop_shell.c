#include "shell.h"

void free_commands(t_command* command) {
    int i;
    while (command != NULL) {
        t_command* next = command->next_command;
        i = 0;
        while (command->args[i] != NULL) {
            free(command->args[i]);
            i++;
        }
        free(command->args);
        free(command);
        command = next;
    }
}

int loop_shell(void) {
    t_command* first_command;
    char* line;
    while (1) {
        line = get_next_line(STDIN_FILENO);
        first_command = ming_parse(ming_tokenizer(line));
        int status = ming_execute(first_command);
        if (status == 1) break;
        else if (status != 0) {
            perror("Error executing in loop shell");
            return -1;
        }

        free(line);
        free_commands(first_command);
        break;
    }
    return 0;
}
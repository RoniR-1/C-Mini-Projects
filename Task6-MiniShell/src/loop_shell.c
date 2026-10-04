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
        //free(command.)
    }
}

int loop_shell(void) {
    t_command* first_command;
    char* line;
    while (1) {
        line = get_next_line(STDIN_FILENO);
        first_command = ming_parse(ming_tokenizer(line));
        if (ming_execute(first_command) != 0) perror("Error executing in loop shell");
        break;
    }
    printf("%d", first_command->fd_in);
    free(line);
    return 0;
}
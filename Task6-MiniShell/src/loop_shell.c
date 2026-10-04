#include "shell.h"

int loop_shell(void) {
    t_command* first_command;
    char* line;
    while (1) {
        line = get_next_line(STDIN_FILENO);
        first_command = ming_parse(ming_tokenizer(line));
        break;
    }
    printf("%d", first_command->fd_in);
    free(line);
    return 0;
}
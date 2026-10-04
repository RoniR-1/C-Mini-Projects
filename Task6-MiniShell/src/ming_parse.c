#include "shell.h"

t_command* allocate_command(void) {
    t_command* command = calloc(sizeof(t_command), 1);
    if (command == NULL) perror("Failed calloc of allocate_command");
    return command;
}


/*
    Appends s2 to s1, so that result is s1 + s2
    if s2 == null, return s1
    if s1 == null, allocate new memory and join s1 and s2
*/
char* string_join(char* s1, char* s2) {
    int len1 = strlength(s1);
    int len2 = strlength(s2);
    if (s2 == NULL) return s1;
    if (s1 == NULL) {
        s1 = calloc(sizeof(char), len2+1);
        if (s1 == NULL) perror("Failed calloc on string_join");
    }
}


t_command* ming_parse(t_token* token) {
    if (token == NULL) perror("ming parses received null token");
    t_command* first_command = allocate_command();
    t_command* current_command = first_command;
    
    do {
        if (token->type == TOKEN_WORD) {
            strnJoin(current_command->args, token->value);
        }
        else if (token->type == TOKEN_PIPE) {
            current_command->next_command = allocate_command();
            current_command = current_command->next_command;
        }
        




        token = token->next;
    } while(token != NULL);

    return first_command;
}
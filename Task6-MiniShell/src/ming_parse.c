#include "shell.h"

void free_tokens(t_token* token) {
    while (token != NULL) {
        t_token* next = token->next;
        free(token->value);
        free(token);
        token = next;
    }
}

t_command* allocate_command(void) {
    t_command* command = calloc(sizeof(t_command), 1);
    if (command == NULL) {
        perror("Failed calloc of allocate_command");
        return NULL;
    }
    command->fd_in = STDIN_FILENO;
    command->fd_out = STDOUT_FILENO;
    return command;
}

int array_length(char **arr) {
    int count = 0;
    while (arr && arr[count]) count++;
    return count;
}

char* string_join(char* s1, char* s2) {
    if (s2 == NULL) return s1;
    int len1 = strlength(s1);
    int len2 = strlength(s2);
    char* result = calloc(sizeof(char), len1 + len2 + 1);
    int left = 0; int right = 0;
    while (s1 && s1[left]) {
        result[left] = s1[left];
        left++;
    }
    while (s2 && s2[right]) {
        result[left+right] = s2[right];
        right++;
    }
    result[left+right] = 0;
    free(s1);
    return result;
}

/*
    Appends s2 to s1, so that result is s1, s2
    if s2 == null, return s1
    else allocate new memory and join s1 and s2, acccepts s1 being null
*/
char** string_add(char** s1, char* s2) {
    if (s2 == NULL) return s1;
    int len1 = array_length(s1);
    int len2 = strlength(s2);
    char** result = calloc(sizeof(char*), len1 + len2 + 1);
    int left = 0;
    while (s1 != NULL && s1[left] != NULL) {
        result[left] = s1[left];
        left++;
    }
    result[left] = string_join(NULL, s2);
    left++;
    result[left] = NULL;
    free(s1);
    return result;
}


t_command* ming_parse(t_token* token) {
    if (token == NULL) {
        perror("ming parses received null token");
        return NULL;
    }
    t_token* first_token = token;
    t_command* first_command = allocate_command();
    t_command* current_command = first_command;
    
    do {
        if (token->type == TOKEN_WORD) {
            current_command->args = string_add(current_command->args, token->value);
        }
        else if (token->type == TOKEN_PIPE) {
            current_command->next_command = allocate_command();
            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("Failed to create pipe");
                return NULL;
            }
            current_command->fd_out = pipefd[1];
            current_command->next_command->fd_in = pipefd[0];
            current_command = current_command->next_command ;
        }
        else {
            char* filename;
            int fd;
            int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; 
            t_token_type type = token->type;
            if (token->next == NULL) {
                perror("MingParsing encounterd <,<<,> or >> without a next token");
                return NULL;
            }
            token = token->next;
            filename = token->value;

            if (type == REDIR_IN) {
                if (current_command->fd_in != STDIN_FILENO) {
                    close(current_command->fd_in);
                }
                fd = open(filename, O_RDONLY);
                if (fd == -1) {
                    perror("Failed opening file in ming parser REDIR_IN");
                    return NULL;
                }
                current_command->fd_in = fd;
            }
            else if (type == HEREDOC) {
                // dont care
            }
            else {
                if (current_command->fd_out != STDOUT_FILENO) {
                    close(current_command->fd_out);
                }
                int flags = O_WRONLY | O_CREAT;
                flags |= type == REDIR_OUT ? O_TRUNC : O_APPEND;
                current_command->fd_out = open(filename, flags, mode);
                if (current_command->fd_out == -1) {
                    perror("Failed opening file in ming parser REDIR_IN");
                    return NULL;
                }
            }
        }
        token = token->next;
    } while(token != NULL);

    free_tokens(first_token);
    return first_command;
}
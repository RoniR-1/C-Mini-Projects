#include "shell.h"


t_token* allocate_token(void) {
    t_token * token = calloc(sizeof(t_token), 1);
    if (token == NULL) perror("Failed token calloc in allocate_token");
    return token;
}

t_token* tokonize_word(char* s, t_token* current_token, int right, int* left) {
    if (*left == -1) return current_token;
    
    current_token->type = TOKEN_WORD;
    current_token->value = malloc(sizeof(char) * (right - (*left) + 1));
    if (current_token->value == NULL) perror("Failed malloc for token value in tokonize_word");
    
    int j = *left;
    int k = 0;
    while (j < right) {
        if (s[j] != '\"' && s[j] != '\'') {
            current_token->value[k] = s[j];
            k++;
        }
        j++;
    }
    current_token->value[k] = '\0';

    *left = -1;
    current_token->next = allocate_token();
    return current_token->next;
}

// placeholder until i feel like implementing expanding
void expand() {}

t_token* tokenize_general(char* s,t_token* current_token, int* ii, int* first_char_seen) {
    int i = *ii;
    if (s[i] == ' ') {
        current_token = tokonize_word(s, current_token, i, first_char_seen);
    }
    else if (s[i] == '$') {
        expand();
    }
    else if (s[i] == '|' || s[i] == '<' || s[i] == '>') {
        current_token = tokonize_word(s, current_token, i, first_char_seen);

        if (s[i] == '|') current_token->type = TOKEN_PIPE;
        else if (s[i] == '>') {
            if (s[i+1] == '>') {
                current_token->type = TOKEN_REDIR_APPEND;
                (*ii)++;
            }
            else current_token->type = TOKEN_REDIR_OUT;
        }
        else if (s[i] == '<') {
            if (s[i+1] == '<') {
                current_token->type = TOKEN_HEREDOC;
                (*ii)++;
            }
            else current_token->type = TOKEN_REDIR_IN;
        }
        current_token->next = allocate_token();
        current_token = current_token->next; // move to the next token
    }
    return current_token;
}


t_token* ming_tokenizer(char* s) {
    t_tokenizer_state token_state = STATE_GENERAL;
    t_token*    first_token = allocate_token();
    t_token*    current_token = first_token;
    int i = 0;
    int first_char_seen = -1;

    while (1) {
        if (s[i] == 0) {
            tokonize_word(s, current_token, i, &first_char_seen);
            break;
        }
        if (s[i] == '\'') {
            if (token_state == STATE_IN_SINGLE) token_state = STATE_GENERAL;
            else if (token_state == STATE_GENERAL) token_state = STATE_IN_SINGLE;
        }
        if (s[i] == '\"') {
            if (token_state == STATE_IN_DOUBLE) token_state = STATE_GENERAL;
            else if (token_state == STATE_GENERAL) token_state = STATE_IN_DOUBLE;
        }

        // STATES   ------------------------------------------------------------------------------------------------
        if (token_state == STATE_GENERAL) { 
            current_token = tokenize_general(s, current_token, &i, &first_char_seen);
        }
        
        else if (token_state == STATE_IN_DOUBLE) {
            if (s[i] == '$') {
                expand();
            }
        }
        else {
            // nothing
        }
        if (first_char_seen == -1 && s[i] != ' ' && s[i] != '|' && s[i] != '<' && s[i] != '>') {
            first_char_seen = i;
        }
        i++;
    }
    if (token_state != STATE_GENERAL) {
        //perror("tokenizer ended outside state general");
    }
    
    // remove tail as it doesnt hold anything
    current_token = first_token;
    if (current_token != NULL) {
        while (current_token->next != NULL && current_token->next->next != NULL) {
            current_token = current_token->next;
        }
        if (current_token->next != NULL && current_token->next->value == NULL && current_token->next->type == TOKEN_WORD) {
            free(current_token->next);
            current_token->next = NULL;
        }
    }

    return first_token;
}

#ifndef MING_TOKENIZER_H
# define MING_TOKENIZER_H

typedef enum {
    STATE_GENERAL = 0,
    STATE_IN_SINGLE,
    STATE_IN_DOUBLE
} t_tokenizer_state;

typedef enum {
    TOKEN_WORD,          // cat
    TOKEN_PIPE,          // |
    TOKEN_REDIR_OUT,     // >
    TOKEN_REDIR_APPEND,  // >>
    TOKEN_REDIR_IN,      // <
    TOKEN_HEREDOC        // <<
} t_token_type;

typedef struct s_token {
    char*           value; 
    t_token_type    type;   
    struct s_token* next;  
} t_token;

#endif
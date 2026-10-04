#ifndef SHELL_H
# define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "getNext.h"
#include "libft.h"
#include "ming_tokenizer.h"

typedef enum {
    REDIR_IN = 1,       // O_RDONLY
    REDIR_OUT,          // O_WRONLY | O_CREAT | O_TRUNC
    REDIR_APPEND,       // O_WRONLY | O_CREAT | O_APPEND
    HEREDOC             // i dont care about it tbh
} t_redir_type; 

typedef struct s_redirection {
    t_redir_type    redir;  //1 = REDIR_IN (<), 2 = REDIR_OUT (>) ,3 = REDIR_APPEND (>>), 4 = HEREDOC (<<)
    char*           target;
    struct s_redirection*  next;
} t_redirection;

typedef struct s_command {
    int                fd_in;
    int                fd_out;
    char**              args;
    t_redirection*      redirection;
    struct s_command*   next_command;
} t_command;




int loop_shell(void);
t_token* ming_tokenizer(char* s);
t_command* ming_parse(t_token* token);


#endif
#ifndef SHELL_H
# define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "getNext.h"
#include "libft.h"
#include "ming_tokenizer.h"

typedef enum {
    REDIR_IN = 1,       // O_RDONLY
    REDIR_OUT,          // O_WRONLY | O_CREAT | O_TRUNC
    REDIR_APPEND,       // O_WRONLY | O_CREAT | O_APPEND
    HEREDOC             // i dont care about it tbh
} t_redir_type; 

typedef struct s_command {
    int                fd_in;
    int                fd_out;
    char**              args;
    struct s_command*   next_command;
} t_command;

#include "ming_calls.h"




int loop_shell(void);
t_token* ming_tokenizer(char* s);
t_command* ming_parse(t_token* token);
int ming_execute(t_command* command);

#endif
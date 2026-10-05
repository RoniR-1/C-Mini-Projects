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

#define EXIT_MING_CODE 42

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
int ming_launch(t_command* command);

#endif
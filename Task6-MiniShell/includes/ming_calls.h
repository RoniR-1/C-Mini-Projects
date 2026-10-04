#ifndef MING_CALLS_H
# define MING_CALLS_H

#include "shell.h"

int builtin_index(t_command* command);
int execute_builtin(t_command* command);

#endif
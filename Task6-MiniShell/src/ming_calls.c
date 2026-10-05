#include "shell.h"

int ming_cd(t_command* command);
int ming_help(t_command* command);
int ming_exit(t_command* command);

char* built_in_call_str[] = {
    "cd",
    "help",
    "exit",
    NULL
};

int (*builtin_func[]) (t_command* command) = {
    &ming_cd,
    &ming_help,
    &ming_exit,
    NULL
};

int builtin_index(t_command* command) {
    if (!command || !command->args || !command->args[0]) return -1;

    int index = 0;
    int size = strlength(command->args[0]);
    while(built_in_call_str[index] != NULL) {
        if (strlength(built_in_call_str[index]) == size 
            && ft_strncmp(command->args[0], built_in_call_str[index], size) == 0) {
            return index;
        }
        index++;
    }
    return -1;
}

int execute_builtin(t_command* command) {
    int index = builtin_index(command);
    if (index == -1) {
        perror("Tried executing builtin function with index -1");
        return 0;
    }
    return builtin_func[index](command);
}

int ming_cd(t_command* command) {
    if (command == NULL || command->args == NULL || command->args[0] == NULL) return -1;
    if (chdir(command->args[1]) == -1) {
        perror("Failed cd in ming_cd");
        return 0;
    }
    return 0;
}

int ming_help(t_command* command) {
    char *msg = "Welcome to Ming Shell.\nHope you like it!\nCurrently Built-in functions are:\n1.exit: exits MingShell\n2.cd: changes directory\n3.help: helps you\n";
    if (command->fd_out == -1) {
        perror("File desciptor is negative in ming_help");
        return 0;
    }
    int size = ft_strlen(msg);
    if (write(command->fd_out, msg, size) != size) {
        perror("Failed writing ming help");
        return 0;
    }
    return 0;
}

int ming_exit(t_command* command) {
    return EXIT_MING_CODE;
}


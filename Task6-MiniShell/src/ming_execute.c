#include "shell.h"


int ming_launch(t_command* command) {
    pid_t pid, wpid;
    int status;

    pid = fork(); // 0 for child, pid for parent
    if (pid == -1) {
        perror("Failed forking in ming_launch");
        return -1;
    }
    else if (pid == 0) {
        if (execvp(command->args[0], command->args) == -1) {
            perror("Error from execvp in ming_launch");
        }
        exit(EXIT_FAILURE);
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

}

/*
    Executes commands. Returns 0 on sucess, 1 on exit, else on error
*/
int ming_execute(t_command* command) {
    while (command != NULL) {
        if (builtin_index(command) != -1) {
            if (execute_builtin(command) == -1) return 0;
        }
        else {
            if (ming_launch(command) != 0) {
                perror("Ming program launcher failed");
                return 0;
            }
        }
        command = command->next_command;
    }
    return 0;
}
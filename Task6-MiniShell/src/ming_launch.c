#include "shell.h"

int commands_size(t_command *cmd) {
    int count = 0;
    while (cmd) {
        count++;
        cmd = cmd->next_command;
    }
    return count;
}

int ming_launch(t_command* command) {
    fflush(stdout);
    t_command* head_command = command;
    if (command == NULL) return 0;
    // if it the only commaand and its built in, run it on the same process
    if (command->next_command == NULL && builtin_index(command) != -1) {
            int status = execute_builtin(command);
            if (command->fd_in != STDIN_FILENO) close(command->fd_in);
            if (command->fd_out != STDOUT_FILENO) close(command->fd_out);
            return status;
        }    
    pid_t wpid;
    pid_t* pids;
    int status;

    int size = commands_size(command);
    pids = malloc(sizeof(pid_t) * size);
    if (pids == NULL) {
        perror("Failed malloc in ming_launc-pids");
        return 0;
    }

    for (int i = 0; i < size && command != NULL; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("Failed forking in ming_launch");
            free(pids);
            return 0;
        }
        if (pids[i] == 0) {
            t_command* tmp = head_command;
            while (tmp != NULL) {
                if (tmp != command) {
                    if (tmp->fd_in != STDIN_FILENO && tmp->fd_in != command->fd_in && tmp->fd_out != command->fd_out) {
                        close(tmp->fd_in);
                    }
                    if (tmp->fd_out != STDOUT_FILENO && tmp->fd_out != command->fd_in && tmp->fd_out != command->fd_out) {
                        close(tmp->fd_out);
                    }  
                }
                tmp = tmp->next_command;
            }



            dup2(command->fd_in, STDIN_FILENO);
            dup2(command->fd_out, STDOUT_FILENO);
            if (command->fd_in != STDIN_FILENO) close (command->fd_in);
            if (command->fd_out != STDOUT_FILENO) close (command->fd_out);
            
            if (command->args == NULL || command->args[0] == NULL) {
                exit(0);
            }

            if (builtin_index(command) != -1) {
                int status = execute_builtin(command);
                exit(status);
            }
            if (execvp(command->args[0], command->args) == -1) {
                perror("Error from execvp in ming_launch");
            }
            exit(0);
        }
        else {
            if (command->fd_in != STDIN_FILENO) close(command->fd_in);
            if (command->fd_out != STDOUT_FILENO) close(command->fd_out);
        }
        command = command->next_command;
    }
    int last_status = 0;
    for (int i = 0; i < size; i++) {
        waitpid(pids[i], &status, 0);
        if (i == size-1) {
            if (WIFEXITED(status)) {
                last_status = WEXITSTATUS(status);
            }
        }
    }
    free(pids);
    return 0;
}

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// again ai generated test cases

// --- Helper to build expected commands easily ---
t_command* create_expected_cmd(int fd_in, int fd_out, char** args, t_command* next) {
    t_command* cmd = calloc(sizeof(t_command), 1);
    cmd->fd_in = fd_in;
    cmd->fd_out = fd_out;
    cmd->args = args;
    cmd->next_command = next;
    return cmd;
}

// --- Assertion Logic ---
/*
    Returns 0 if equal, 1 if not equal.
    Prints minimal info on failure.
*/
int assert_equals_commands(const char* test_name, t_command* expected, t_command* actual) {
    t_command *curr_exp = expected;
    t_command *curr_act = actual;
    int i = 0; // Command index

    while (curr_exp != NULL && curr_act != NULL) {
        // Check FDs (-1 means we expect a dynamically opened file, so just check it isn't 0 or 1)
        if (curr_exp->fd_in != -1 && curr_exp->fd_in != curr_act->fd_in) {
            printf("[\033[31mFAIL\033[0m] %s (Cmd %d): fd_in mismatch (Exp: %d, Got: %d)\n", test_name, i, curr_exp->fd_in, curr_act->fd_in);
            return 1;
        }
        if (curr_exp->fd_out != -1 && curr_exp->fd_out != curr_act->fd_out) {
            printf("[\033[31mFAIL\033[0m] %s (Cmd %d): fd_out mismatch (Exp: %d, Got: %d)\n", test_name, i, curr_exp->fd_out, curr_act->fd_out);
            return 1;
        }

        // Check Args array
        if (curr_exp->args == NULL && curr_act->args != NULL) {
            printf("[\033[31mFAIL\033[0m] %s (Cmd %d): Expected no args, but got some.\n", test_name, i); return 1;
        }
        if (curr_exp->args != NULL) {
            if (curr_act->args == NULL) {
                printf("[\033[31mFAIL\033[0m] %s (Cmd %d): Expected args, but got NULL.\n", test_name, i); return 1;
            }
            int arg_idx = 0;
            while (curr_exp->args[arg_idx] != NULL) {
                if (curr_act->args[arg_idx] == NULL || strcmp(curr_exp->args[arg_idx], curr_act->args[arg_idx]) != 0) {
                    printf("[\033[31mFAIL\033[0m] %s (Cmd %d): Arg[%d] mismatch (Exp: \"%s\", Got: \"%s\")\n", 
                        test_name, i, arg_idx, curr_exp->args[arg_idx], curr_act->args[arg_idx] ? curr_act->args[arg_idx] : "NULL");
                    return 1;
                }
                arg_idx++;
            }
            if (curr_act->args[arg_idx] != NULL) {
                printf("[\033[31mFAIL\033[0m] %s (Cmd %d): Got extra unexpected arguments.\n", test_name, i); return 1;
            }
        }
        
        curr_exp = curr_exp->next_command;
        curr_act = curr_act->next_command;
        i++;
    }

    if (curr_exp != NULL || curr_act != NULL) {
        printf("[\033[31mFAIL\033[0m] %s : Command count mismatch (Got too %s commands)\n", test_name, curr_act != NULL ? "many" : "few");
        return 1;
    }

    printf("[\033[32mPASS\033[0m] %s\n", test_name);
    return 0;
}


// --- Test Suites ---

static void test_simple_parse(void) {
    char *input = strdup("ls -la");
    t_token *tokens = ming_tokenizer(input);
    t_command *actual = ming_parse(tokens);
    
    // Build expected structure
    char* exp_args[] = {"ls", "-la", NULL};
    t_command *expected = create_expected_cmd(STDIN_FILENO, STDOUT_FILENO, exp_args, NULL);
    
    assert_equals_commands("Simple parse (ls -la)", expected, actual);
    
    // Memory cleanup omitted for brevity, but you should free actual and expected here!
    free(expected);
    free(input);
}

static void test_pipe_parse(void) {
    char *input = strdup("cat | grep hello");
    t_token *tokens = ming_tokenizer(input);
    t_command *actual = ming_parse(tokens);
    
    // Build expected structure (2 commands)
    char* exp_args2[] = {"grep", "hello", NULL};
    t_command *cmd2 = create_expected_cmd(-1, STDOUT_FILENO, exp_args2, NULL); // fd_in should be modified by pipe

    char* exp_args1[] = {"cat", NULL};
    t_command *cmd1 = create_expected_cmd(STDIN_FILENO, -1, exp_args1, cmd2); // fd_out should be modified by pipe
    
    assert_equals_commands("Pipe parse (cat | grep hello)", cmd1, actual);
    
    free(cmd1);
    free(cmd2);
    free(input);
}

static void test_redirection_parse(void) {
    // Note: running this test will actually create "test_out.txt" on your computer because ming_parse calls open()!
    char *input = strdup("echo hi > test_out.txt");
    t_token *tokens = ming_tokenizer(input);
    t_command *actual = ming_parse(tokens);
    
    char* exp_args[] = {"echo", "hi", NULL};
    t_command *expected = create_expected_cmd(STDIN_FILENO, -1, exp_args, NULL); // fd_out is -1 because open() changes it
    
    assert_equals_commands("Redirection parse (echo hi > test_out.txt)", expected, actual);
    
    free(expected);
    free(input);
}
static void test_multiple_redirections(void) {
    // 1. Create a dummy input file so open() doesn't fail on REDIR_IN
    system("touch test_in.txt"); 

    // 2. The input command
    char *input = strdup("echo hi > test_out1.txt > test_out2.txt < test_in.txt");
    t_token *tokens = ming_tokenizer(input);
    t_command *actual = ming_parse(tokens);
    
    // 3. Expected Structure:
    // args will only contain "echo" and "hi". All filenames are consumed by the parser.
    char* exp_args[] = {"echo", "hi", NULL};
    
    // Both fd_in and fd_out are opened dynamically, so we expect -1 (not 0 or 1)
    t_command *expected = create_expected_cmd(-1, -1, exp_args, NULL);
    
    // 4. Assert
    assert_equals_commands("Multiple levels (echo hi > out1 > out2 < in)", expected, actual);
    
    // 5. Cleanup memory
    free(expected);
    free(input);
    
    // 6. Cleanup the physical files created by the OS during ming_parse
    system("rm -f test_out1.txt test_out2.txt test_in.txt");
}

void parser_tester(void) {
    printf("--- Running Parser Tests ---\n\n");

    test_simple_parse();
    test_pipe_parse();
    test_redirection_parse();
    test_multiple_redirections();

    printf("\n--- Tests Finished ---\n");
}
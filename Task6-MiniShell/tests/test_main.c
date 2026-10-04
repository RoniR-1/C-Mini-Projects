#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

// ai generated to help develop the tokenizer

// --- Test Helper Functions ---

static const char* token_type_to_str(t_token_type type) {
    switch (type) {
        case TOKEN_WORD:         return "TOKEN_WORD";
        case TOKEN_PIPE:         return "TOKEN_PIPE";
        case TOKEN_REDIR_OUT:    return "TOKEN_REDIR_OUT";
        case TOKEN_REDIR_APPEND: return "TOKEN_REDIR_APPEND";
        case TOKEN_REDIR_IN:     return "TOKEN_REDIR_IN";
        case TOKEN_HEREDOC:      return "TOKEN_HEREDOC";
        default:                 return "UNKNOWN";
    }
}

static void free_tokens(t_token *tokens) {
    t_token *tmp;

    while (tokens) {
        tmp = tokens->next;
        if (tokens->value)
            free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}

typedef struct s_expected_token {
    char         *value;
    t_token_type  type;
} t_expected_token;

static int assert_tokens(const char *test_name, const char *input_literal, t_expected_token *expected, int count) {
    // Copy read-only string literal into writable heap memory
    char *input = strdup(input_literal);
    if (!input) return 0;

    t_token *actual = ming_tokenizer(input);
    t_token *curr = actual;
    int i = 0;
    int passed = 1;

    while (curr && i < count) {
        if (curr->type != expected[i].type) {
            passed = 0;
            break;
        }
        
        // If both are supposed to be words, compare strings
        if (expected[i].value != NULL) {
            if (curr->value == NULL || strcmp(curr->value, expected[i].value) != 0) {
                passed = 0;
                break;
            }
        } else {
            // If it's an operator, expect the value to be exactly NULL
            if (curr->value != NULL) {
                passed = 0;
                break;
            }
        }
        
        curr = curr->next;
        i++;
    }

    if (curr != NULL || i != count)
        passed = 0;

    if (passed) {
        printf("[\033[32mPASS\033[0m] %s (\"%s\")\n", test_name, input_literal);
    } else {
        printf("[\033[31mFAIL\033[0m] %s (\"%s\")\n", test_name, input_literal);
        printf("  Expected:\n");
        for (int j = 0; j < count; j++) {
            // Use ternary operator to safely print NULL expected values
            printf("    [%d] type: %-18s value: \"%s\"\n",
                   j, token_type_to_str(expected[j].type), expected[j].value ? expected[j].value : "NULL");
        }
        printf("  Got:\n");
        curr = actual;
        int k = 0;
        while (curr) {
            // Use ternary operator to safely print NULL actual values
            printf("    [%d] type: %-18s value: \"%s\"\n",
                   k, token_type_to_str(curr->type), curr->value ? curr->value : "NULL");
            curr = curr->next;
            k++;
        }
    }

    free_tokens(actual);
    free(input); // Free the writable copy
    return passed;
}

// --- Test Suites ---

static void test_simple_command(void) {
    t_expected_token expected[] = {
        {"ls", TOKEN_WORD},
        {"-la", TOKEN_WORD}
    };
    assert_tokens("Simple command with arguments", "ls -la", expected, 2);
}

static void test_pipes_and_redirections(void) {
    t_expected_token expected[] = {
        {"cat", TOKEN_WORD},
        {"file.txt", TOKEN_WORD},
        {NULL, TOKEN_PIPE},              // Operator has NULL value
        {"grep", TOKEN_WORD},
        {"hello", TOKEN_WORD},
        {NULL, TOKEN_REDIR_OUT},         // Operator has NULL value
        {"out.txt", TOKEN_WORD}
    };
    assert_tokens("Pipes and single redirection", "cat file.txt | grep hello > out.txt", expected, 7);
}

static void test_double_redirections(void) {
    t_expected_token expected[] = {
        {"echo", TOKEN_WORD},
        {"hi", TOKEN_WORD},
        {NULL, TOKEN_REDIR_APPEND},      // Operator has NULL value
        {"log.txt", TOKEN_WORD},
        {NULL, TOKEN_HEREDOC},           // Operator has NULL value
        {"EOF", TOKEN_WORD}
    };
    assert_tokens("Append and heredoc redirections", "echo hi >> log.txt << EOF", expected, 6);
}

static void test_unspaced_operators(void) {
    t_expected_token expected[] = {
        {"cat", TOKEN_WORD},
        {NULL, TOKEN_REDIR_IN},          // Operator has NULL value
        {"in", TOKEN_WORD},
        {NULL, TOKEN_PIPE},              // Operator has NULL value
        {"wc", TOKEN_WORD},
        {NULL, TOKEN_REDIR_APPEND},      // Operator has NULL value
        {"out", TOKEN_WORD}
    };
    assert_tokens("Operators without spaces", "cat<in|wc>>out", expected, 7);
}

static void test_quotes(void) {
    t_expected_token expected[] = {
        {"echo", TOKEN_WORD},
        {"hello world", TOKEN_WORD},
        {NULL, TOKEN_PIPE},              // Operator has NULL value
        {"cat", TOKEN_WORD}
    };
    assert_tokens("Quoted strings containing spaces/operators", "echo \"hello world\" | cat", expected, 4);
}

int main(void) {
    printf("--- Running Tokenizer Tests ---\n\n");

    test_simple_command();
    test_pipes_and_redirections();
    test_double_redirections();
    test_unspaced_operators();
    test_quotes();

    printf("\n--- Tests Finished ---\n");
    return 0;
}
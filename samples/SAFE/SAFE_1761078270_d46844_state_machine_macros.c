//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10
#define MAX_TOKEN_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) (isalnum((unsigned char)c) || (c) == ' ' || (c) == ',' || (c) == '.')

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int count;
} TokenList;

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

static void initialize_tokens(TokenList *tl) {
    tl->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        tl->tokens[i][0] = '\0';
    }
}

static int add_token(TokenList *tl, const char *token, size_t len) {
    if (tl->count >= MAX_TOKENS || len >= MAX_TOKEN_LEN) return 0;
    if (strncpy(tl->tokens[tl->count], token, len) == NULL) return 0;
    tl->tokens[tl->count][len] = '\0';
    tl->count++;
    return 1;
}

static void process_tokens(const TokenList *tl) {
    printf("Processing %d tokens:\n", tl->count);
    for (int i = 0; i < tl->count; i++) {
        printf("  Token %d: '%s'\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    TokenList tokens;
    int current_state = STATE_IDLE;
    size_t input_len = 0;
    size_t token_start = 0;
    int in_token = 0;
    
    printf("Enter text to tokenize (alphanumeric, spaces, commas, periods): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
    }
    
    if (current_state != STATE_ERROR) {
        if (!validate_input(input)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    initialize_tokens(&tokens);
    
    if (current_state == STATE_READING) {
        for (size_t i = 0; i <= input_len; i++) {
            char c = input[i];
            if (isalnum((unsigned char)c)) {
                if (!in_token) {
                    token_start = i;
                    in_token = 1;
                }
            } else {
                if (in_token) {
                    size_t token_len = i - token_start;
                    if (!add_token(&tokens, &input[token_start], token_len)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    in_token = 0;
                }
            }
        }
        if (current_state != STATE_ERROR) {
            TRANSITION(STATE_PROCESSING);
        }
    }
    
    if (current_state == STATE_PROCESSING) {
        if (tokens.count > 0) {
            process_tokens(&tokens);
            TRANSITION(STATE_DONE);
        } else {
            TRANSITION(STATE_ERROR);
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Tokenization completed successfully.\n");
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error.\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
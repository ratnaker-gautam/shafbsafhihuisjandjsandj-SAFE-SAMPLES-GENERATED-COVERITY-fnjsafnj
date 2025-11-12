//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
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

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

static void tokenize_input(const char* input, TokenList* tokens) {
    tokens->count = 0;
    const char* ptr = input;
    char current_token[MAX_TOKEN_LEN];
    int token_len = 0;
    
    while (*ptr && tokens->count < MAX_TOKENS) {
        if (isspace((unsigned char)*ptr)) {
            if (token_len > 0) {
                if (token_len < MAX_TOKEN_LEN) {
                    memcpy(tokens->tokens[tokens->count], current_token, token_len);
                    tokens->tokens[tokens->count][token_len] = '\0';
                    tokens->count++;
                }
                token_len = 0;
            }
        } else {
            if (token_len < MAX_TOKEN_LEN - 1) {
                current_token[token_len++] = *ptr;
            }
        }
        ptr++;
    }
    
    if (token_len > 0 && tokens->count < MAX_TOKENS) {
        if (token_len < MAX_TOKEN_LEN) {
            memcpy(tokens->tokens[tokens->count], current_token, token_len);
            tokens->tokens[tokens->count][token_len] = '\0';
            tokens->count++;
        }
    }
}

static void process_tokens(const TokenList* tokens) {
    for (int i = 0; i < tokens->count; i++) {
        printf("Token %d: %s\n", i + 1, tokens->tokens[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int current_state = STATE_IDLE;
    TokenList tokens;
    
    printf("Enter text to tokenize (alphanumeric, spaces, commas, periods only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (!validate_input(input)) {
                    printf("Error: Invalid input\n");
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                tokenize_input(input, &tokens);
                if (tokens.count == 0) {
                    printf("No tokens found\n");
                } else {
                    printf("Found %d tokens:\n", tokens.count);
                    process_tokens(&tokens);
                }
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_ERROR:
                printf("Processing failed\n");
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
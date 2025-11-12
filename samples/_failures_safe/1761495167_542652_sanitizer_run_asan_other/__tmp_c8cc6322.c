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
#define TOKEN_LEN 20

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) (isalnum((unsigned char)c) || (c) == ' ' || (c) == ',' || (c) == '.')

typedef struct {
    char tokens[MAX_TOKENS][TOKEN_LEN];
    int count;
} TokenList;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

void initialize_tokens(TokenList* tl) {
    tl->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        memset(tl->tokens[i], 0, TOKEN_LEN);
    }
}

int add_token(TokenList* tl, const char* token) {
    if (tl->count >= MAX_TOKENS) return 0;
    size_t len = strlen(token);
    if (len == 0 || len >= TOKEN_LEN) return 0;
    strncpy(tl->tokens[tl->count], token, TOKEN_LEN - 1);
    tl->tokens[tl->count][TOKEN_LEN - 1] = '\0';
    tl->count++;
    return 1;
}

void process_tokens(const TokenList* tl) {
    printf("Processing %d tokens:\n", tl->count);
    for (int i = 0; i < tl->count; i++) {
        printf("  Token %d: '%s'\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    TokenList tokens;
    int current_state = STATE_IDLE;
    char* token;
    char* saveptr;
    
    printf("Enter text to tokenize (alphanumeric, spaces, commas, periods): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state != STATE_ERROR) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (!validate_input(input)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    if (current_state == STATE_READING) {
        initialize_tokens(&tokens);
        token = strtok_r(input, " ,.", &saveptr);
        
        while (token != NULL && current_state == STATE_READING) {
            if (!add_token(&tokens, token)) {
                TRANSITION(STATE_ERROR);
                break;
            }
            token = strtok_r(NULL, " ,.", &saveptr);
        }
        
        if (current_state == STATE_READING) {
            if (tokens.count > 0) {
                TRANSITION(STATE_PROCESSING);
            } else {
                TRANSITION(STATE_ERROR);
            }
        }
    }
    
    if (current_state == STATE_PROCESSING) {
        process_tokens(&tokens);
        TRANSITION(STATE_DONE);
    }
    
    switch (current_state) {
        case STATE_DONE:
            printf("Tokenization completed successfully.\n");
            break;
        case STATE_ERROR:
            printf("Error: Invalid input or processing error.\n");
            break;
        default:
            printf("Unexpected state: %d\n", current_state);
            break;
    }
    
    return (current_state == STATE_DONE) ? EXIT_SUCCESS : EXIT_FAILURE;
}
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
#define IS_VALID_CHAR(c) (isalnum((unsigned char)c) || (c) == '_')
#define CHECK_BOUNDS(len, max) ((len) < (max))

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int count;
} TokenList;

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i]) && !isspace((unsigned char)input[i])) {
            return 0;
        }
    }
    return 1;
}

void initialize_tokens(TokenList* tl) {
    tl->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        tl->tokens[i][0] = '\0';
    }
}

int add_token(TokenList* tl, const char* token, int token_len) {
    if (tl->count >= MAX_TOKENS) return 0;
    if (token_len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(tl->tokens[tl->count], token, token_len);
    tl->tokens[tl->count][token_len] = '\0';
    tl->count++;
    return 1;
}

void process_state_machine(const char* input, TokenList* tokens) {
    int current_state = STATE_IDLE;
    char current_token[MAX_TOKEN_LEN];
    int token_pos = 0;
    size_t input_pos = 0;
    size_t input_len = strlen(input);
    
    initialize_tokens(tokens);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (input_pos >= input_len) {
                    TRANSITION(STATE_DONE);
                } else if (isspace((unsigned char)input[input_pos])) {
                    input_pos++;
                } else if (IS_VALID_CHAR(input[input_pos])) {
                    if (CHECK_BOUNDS(token_pos, MAX_TOKEN_LEN-1)) {
                        current_token[token_pos++] = input[input_pos++];
                    }
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (input_pos >= input_len) {
                    if (CHECK_BOUNDS(token_pos, MAX_TOKEN_LEN-1)) {
                        current_token[token_pos] = '\0';
                        if (!add_token(tokens, current_token, token_pos)) {
                            TRANSITION(STATE_ERROR);
                        } else {
                            token_pos = 0;
                            TRANSITION(STATE_DONE);
                        }
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (IS_VALID_CHAR(input[input_pos])) {
                    if (CHECK_BOUNDS(token_pos, MAX_TOKEN_LEN-1)) {
                        current_token[token_pos++] = input[input_pos++];
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (isspace((unsigned char)input[input_pos])) {
                    if (CHECK_BOUNDS(token_pos, MAX_TOKEN_LEN-1)) {
                        current_token[token_pos] = '\0';
                        if (!add_token(tokens, current_token, token_pos)) {
                            TRANSITION(STATE_ERROR);
                        } else {
                            token_pos = 0;
                            input_pos++;
                            TRANSITION(STATE_IDLE);
                        }
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
    }
}

void print_tokens(const TokenList* tokens) {
    if (tokens->count == 0) {
        printf("No tokens found.\n");
        return;
    }
    
    printf("Found %d tokens:\n", tokens->count);
    for (int i = 0; i < tokens->count; i++) {
        printf("  %d: '%s'\n", i + 1, tokens->tokens[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    TokenList tokens;
    
    printf("Enter text to tokenize (alphanumeric and underscore only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input. Only alphanumeric
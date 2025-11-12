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

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

void tokenize_input(const char* input, TokenList* tokens) {
    tokens->count = 0;
    size_t input_len = strlen(input);
    size_t token_start = 0;
    int in_token = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        if (c == '\0' || c == ' ' || c == ',' || c == '.') {
            if (in_token && tokens->count < MAX_TOKENS) {
                size_t token_len = i - token_start;
                if (token_len > 0 && token_len < MAX_TOKEN_LEN) {
                    strncpy(tokens->tokens[tokens->count], &input[token_start], token_len);
                    tokens->tokens[tokens->count][token_len] = '\0';
                    tokens->count++;
                }
                in_token = 0;
            }
        } else {
            if (!in_token) {
                token_start = i;
                in_token = 1;
            }
        }
    }
}

void process_tokens(TokenList* tokens) {
    for (int i = 0; i < tokens->count; i++) {
        for (int j = 0; tokens->tokens[i][j] != '\0'; j++) {
            if (islower((unsigned char)tokens->tokens[i][j])) {
                tokens->tokens[i][j] = toupper((unsigned char)tokens->tokens[i][j]);
            }
        }
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    TokenList tokens;
    
    printf("Enter text to process (alphanumeric, spaces, commas, periods only): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (!validate_input(input_buffer)) {
                    printf("Invalid input detected.\n");
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                tokenize_input(input_buffer, &tokens);
                if (tokens.count == 0) {
                    printf("No valid tokens found.\n");
                    TRANSITION(STATE_ERROR);
                } else {
                    process_tokens(&tokens);
                    TRANSITION(STATE_DONE);
                }
                break;
                
            case STATE_DONE:
                printf("Processing complete. Tokens found: %d\n", tokens.count);
                for (int i = 0; i < tokens.count; i++) {
                    printf("Token %d: %s\n", i + 1, tokens.tokens[i]);
                }
                break;
                
            case STATE_ERROR:
                printf("Error occurred during processing.\n");
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
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

void initialize_tokens(TokenList* tl) {
    tl->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        tl->tokens[i][0] = '\0';
    }
}

int add_token(TokenList* tl, const char* token) {
    if (tl->count >= MAX_TOKENS) return 0;
    size_t len = strlen(token);
    if (len == 0 || len >= MAX_TOKEN_LEN) return 0;
    
    strncpy(tl->tokens[tl->count], token, MAX_TOKEN_LEN - 1);
    tl->tokens[tl->count][MAX_TOKEN_LEN - 1] = '\0';
    tl->count++;
    return 1;
}

void process_tokens(TokenList* tl) {
    for (int i = 0; i < tl->count; i++) {
        for (int j = 0; tl->tokens[i][j] != '\0'; j++) {
            tl->tokens[i][j] = toupper((unsigned char)tl->tokens[i][j]);
        }
    }
}

void print_tokens(const TokenList* tl) {
    printf("Processed tokens (%d):\n", tl->count);
    for (int i = 0; i < tl->count; i++) {
        printf("  %d: %s\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    TokenList tokens;
    int current_state = STATE_IDLE;
    int input_valid = 0;
    
    printf("Enter text to tokenize (alphanumeric, spaces, commas, periods): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        input_valid = validate_input(input);
        if (!input_valid) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    switch (current_state) {
        case STATE_READING:
            initialize_tokens(&tokens);
            
            char* token = strtok(input, " ,.");
            while (token != NULL && tokens.count < MAX_TOKENS) {
                if (!add_token(&tokens, token)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                token = strtok(NULL, " ,.");
            }
            
            if (current_state != STATE_ERROR) {
                if (tokens.count == 0) {
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
            }
            break;
            
        case STATE_PROCESSING:
            process_tokens(&tokens);
            TRANSITION(STATE_DONE);
            break;
            
        case STATE_DONE:
            print_tokens(&tokens);
            break;
            
        case STATE_ERROR:
            printf("Error: Invalid input or processing error\n");
            break;
            
        default:
            printf("Error: Invalid state\n");
            break;
    }
    
    return (current_state == STATE_DONE) ? EXIT_SUCCESS : EXIT_FAILURE;
}
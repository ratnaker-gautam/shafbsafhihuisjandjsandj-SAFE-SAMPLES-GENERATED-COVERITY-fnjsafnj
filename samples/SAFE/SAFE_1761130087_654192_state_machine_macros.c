//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_DONE 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_TOKENS 10

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c) >= 'a' && (c) <= 'z')
#define IS_TERMINATOR(c) ((c) == '\n' || (c) == '\0')

typedef struct {
    char tokens[MAX_TOKENS][MAX_INPUT_LEN];
    int count;
} TokenList;

void initialize_tokens(TokenList *tl) {
    tl->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        tl->tokens[i][0] = '\0';
    }
}

int add_token(TokenList *tl, const char *token) {
    if (tl->count >= MAX_TOKENS) return 0;
    if (strlen(token) >= MAX_INPUT_LEN) return 0;
    
    strncpy(tl->tokens[tl->count], token, MAX_INPUT_LEN - 1);
    tl->tokens[tl->count][MAX_INPUT_LEN - 1] = '\0';
    tl->count++;
    return 1;
}

void process_tokens(TokenList *tl) {
    for (int i = 0; i < tl->count; i++) {
        for (int j = 0; tl->tokens[i][j] != '\0'; j++) {
            if (tl->tokens[i][j] >= 'a' && tl->tokens[i][j] <= 'z') {
                tl->tokens[i][j] = tl->tokens[i][j] - 'a' + 'A';
            }
        }
    }
}

void print_tokens(const TokenList *tl) {
    for (int i = 0; i < tl->count; i++) {
        printf("Token %d: %s\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 2];
    char current_token[MAX_INPUT_LEN];
    int token_pos = 0;
    TokenList tokens;
    
    initialize_tokens(&tokens);
    
    printf("Enter text (a-z only, space separated): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    if (strlen(input) > MAX_INPUT_LEN) {
        TRANSITION(STATE_ERROR);
    }
    
    for (int i = 0; i < (int)strlen(input); i++) {
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(input[i])) {
                    current_token[0] = input[i];
                    current_token[1] = '\0';
                    token_pos = 1;
                    TRANSITION(STATE_READING);
                } else if (input[i] != ' ' && !IS_TERMINATOR(input[i])) {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (IS_VALID_CHAR(input[i])) {
                    if (token_pos < MAX_INPUT_LEN - 1) {
                        current_token[token_pos] = input[i];
                        current_token[token_pos + 1] = '\0';
                        token_pos++;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (input[i] == ' ') {
                    if (!add_token(&tokens, current_token)) {
                        TRANSITION(STATE_ERROR);
                    }
                    TRANSITION(STATE_IDLE);
                } else if (IS_TERMINATOR(input[i])) {
                    if (!add_token(&tokens, current_token)) {
                        TRANSITION(STATE_ERROR);
                    }
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                process_tokens(&tokens);
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_DONE:
                printf("Processed tokens:\n");
                print_tokens(&tokens);
                return 0;
                
            case STATE_ERROR:
                printf("Error: Invalid input\n");
                return 1;
        }
    }
    
    if (current_state == STATE_READING) {
        if (!add_token(&tokens, current_token)) {
            TRANSITION(STATE_ERROR);
        }
        TRANSITION(STATE_PROCESSING);
    }
    
    if (current_state == STATE_PROCESSING) {
        process_tokens(&tokens);
        TRANSITION(STATE_DONE);
    }
    
    if (current_state == STATE_DONE) {
        printf("Processed tokens:\n");
        print_tokens(&tokens);
        return 0;
    }
    
    printf("Error: Invalid state reached\n");
    return 1;
}
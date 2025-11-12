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

void initialize_token_list(TokenList *tl) {
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

void print_tokens(TokenList *tl) {
    for (int i = 0; i < tl->count; i++) {
        printf("Token %d: %s\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 2];
    char current_token[MAX_INPUT_LEN];
    int token_pos = 0;
    TokenList token_list;
    
    initialize_token_list(&token_list);
    
    printf("Enter text (lowercase letters only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (input_len >= sizeof(input_buffer) - 1) {
        TRANSITION(STATE_ERROR);
    }
    
    TRANSITION(STATE_READING);
    
    for (int i = 0; input_buffer[i] != '\0'; i++) {
        char c = input_buffer[i];
        
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (token_pos < MAX_INPUT_LEN - 1) {
                        current_token[token_pos++] = c;
                    } else {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                } else if (c == ' ' || IS_TERMINATOR(c)) {
                    if (token_pos > 0) {
                        current_token[token_pos] = '\0';
                        if (!add_token(&token_list, current_token)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        token_pos = 0;
                    }
                    if (IS_TERMINATOR(c)) {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
            case STATE_DONE:
            case STATE_ERROR:
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_ERROR) break;
    }
    
    if (current_state == STATE_READING && token_pos > 0) {
        current_token[token_pos] = '\0';
        if (!add_token(&token_list, current_token)) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_PROCESSING);
        }
    }
    
    if (current_state == STATE_PROCESSING) {
        if (token_list.count > 0) {
            process_tokens(&token_list);
            TRANSITION(STATE_DONE);
        } else {
            TRANSITION(STATE_ERROR);
        }
    }
    
    switch (current_state) {
        case STATE_DONE:
            printf("Processing complete. Converted to uppercase:\n");
            print_tokens(&token_list);
            break;
            
        case STATE_ERROR:
            printf("Error: Invalid input or processing error\n");
            break;
            
        default:
            printf("Unexpected state: %d\n", current_state);
            break;
    }
    
    return (current_state == STATE_DONE) ? 0 : 1;
}
//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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

void process_tokens(const TokenList *tl) {
    printf("Processing %d tokens:\n", tl->count);
    for (int i = 0; i < tl->count; i++) {
        printf("  Token %d: %s\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 2];
    char current_token[MAX_INPUT_LEN];
    int token_pos = 0;
    TokenList tokens;
    
    initialize_token_list(&tokens);
    
    printf("Enter text (a-z only, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        TRANSITION(STATE_ERROR);
    }
    
    TRANSITION(STATE_READING);
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input_buffer[i];
        
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (token_pos < MAX_INPUT_LEN - 1) {
                        current_token[token_pos++] = c;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (IS_TERMINATOR(c) || c == ' ') {
                    if (token_pos > 0) {
                        current_token[token_pos] = '\0';
                        if (!add_token(&tokens, current_token)) {
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
                process_tokens(&tokens);
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input detected\n");
                return 1;
                
            case STATE_DONE:
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (current_state == STATE_ERROR || current_state == STATE_DONE) {
            break;
        }
    }
    
    if (current_state != STATE_DONE && current_state != STATE_ERROR) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Processing failed\n");
        return 1;
    }
    
    return 0;
}
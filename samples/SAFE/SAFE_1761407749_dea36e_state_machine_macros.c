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

#define TRANSITION(new_state) do { current_state = (new_state); } while(0)
#define IS_VALID_CHAR(c) ((c) >= 'a' && (c) <= 'z')
#define IS_TERMINATOR(c) ((c) == '\n' || (c) == '\0')

typedef struct {
    char tokens[MAX_TOKENS][MAX_INPUT_LEN];
    int count;
} TokenList;

void initialize_token_list(TokenList *list) {
    list->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        list->tokens[i][0] = '\0';
    }
}

int add_token(TokenList *list, const char *token) {
    if (list->count >= MAX_TOKENS) {
        return 0;
    }
    size_t len = strlen(token);
    if (len >= MAX_INPUT_LEN) {
        return 0;
    }
    strncpy(list->tokens[list->count], token, MAX_INPUT_LEN - 1);
    list->tokens[list->count][MAX_INPUT_LEN - 1] = '\0';
    list->count++;
    return 1;
}

void process_tokens(const TokenList *list) {
    printf("Processing %d tokens:\n", list->count);
    for (int i = 0; i < list->count; i++) {
        printf("  Token %d: %s\n", i + 1, list->tokens[i]);
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 2];
    char current_token[MAX_INPUT_LEN];
    int token_pos = 0;
    TokenList tokens;
    
    initialize_token_list(&tokens);
    
    printf("Enter text (lowercase letters only, max %d chars): ", MAX_INPUT_LEN);
    
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
    
    for (size_t i = 0; i <= input_len && current_state != STATE_ERROR && current_state != STATE_DONE; i++) {
        char c = input_buffer[i];
        
        switch (current_state) {
            case STATE_READING:
                if (IS_VALID_CHAR(c)) {
                    if (token_pos < MAX_INPUT_LEN - 1) {
                        current_token[token_pos++] = c;
                        current_token[token_pos] = '\0';
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (IS_TERMINATOR(c) || c == ' ') {
                    if (token_pos > 0) {
                        if (!add_token(&tokens, current_token)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        token_pos = 0;
                        current_token[0] = '\0';
                    }
                    if (IS_TERMINATOR(c)) {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (tokens.count > 0) {
                    process_tokens(&tokens);
                    TRANSITION(STATE_DONE);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return 1;
    }
    
    if (current_state == STATE_DONE) {
        printf("Processing completed successfully\n");
    } else {
        printf("Error: Unexpected termination\n");
        return 1;
    }
    
    return 0;
}
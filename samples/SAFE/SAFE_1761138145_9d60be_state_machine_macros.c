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
#define IS_DIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define IS_ALPHA(ch) (((ch) >= 'a' && (ch) <= 'z') || ((ch) >= 'A' && (ch) <= 'Z'))

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
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
    if (strlen(token) >= MAX_TOKEN_LEN) return 0;
    strcpy(tl->tokens[tl->count], token);
    tl->count++;
    return 1;
}

void print_tokens(const TokenList *tl) {
    printf("Tokens found: %d\n", tl->count);
    for (int i = 0; i < tl->count; i++) {
        printf("  %d: '%s'\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    int current_state = STATE_IDLE;
    TokenList tokens;
    char current_token[MAX_TOKEN_LEN];
    int token_pos = 0;
    int input_pos = 0;
    
    initialize_token_list(&tokens);
    current_token[0] = '\0';
    
    printf("Enter text to tokenize (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    if (current_state != STATE_ERROR) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len > MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (input_pos >= (int)strlen(input)) {
                    if (token_pos > 0) {
                        current_token[token_pos] = '\0';
                        if (!add_token(&tokens, current_token)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        token_pos = 0;
                    }
                    TRANSITION(STATE_PROCESSING);
                } else {
                    char ch = input[input_pos];
                    if (IS_ALPHA(ch) || IS_DIGIT(ch)) {
                        if (token_pos < MAX_TOKEN_LEN - 1) {
                            current_token[token_pos++] = ch;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else {
                        if (token_pos > 0) {
                            current_token[token_pos] = '\0';
                            if (!add_token(&tokens, current_token)) {
                                TRANSITION(STATE_ERROR);
                                break;
                            }
                            token_pos = 0;
                        }
                        input_pos++;
                    }
                }
                break;
                
            case STATE_PROCESSING:
                printf("Processing complete.\n");
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or token limit exceeded.\n");
        return 1;
    }
    
    print_tokens(&tokens);
    return 0;
}
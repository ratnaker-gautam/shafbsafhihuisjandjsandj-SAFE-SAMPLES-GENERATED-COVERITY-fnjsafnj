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

void initialize_token_list(TokenList* tl) {
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
    char input[MAX_INPUT_LEN + 1];
    TokenList token_list;
    int current_state = STATE_IDLE;
    int position = 0;
    char current_token[MAX_TOKEN_LEN];
    int token_pos = 0;
    
    printf("Enter text to tokenize (alphanumeric, spaces, commas, periods only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input)) {
        TRANSITION(STATE_ERROR);
    } else {
        TRANSITION(STATE_READING);
    }
    
    initialize_token_list(&token_list);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (position >= input_len) {
                    if (token_pos > 0) {
                        current_token[token_pos] = '\0';
                        if (!add_token(&token_list, current_token)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        token_pos = 0;
                    }
                    TRANSITION(STATE_PROCESSING);
                    break;
                }
                
                char c = input[position];
                if (c == ' ' || c == ',' || c == '.') {
                    if (token_pos > 0) {
                        current_token[token_pos] = '\0';
                        if (!add_token(&token_list, current_token)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        token_pos = 0;
                    }
                    position++;
                } else if (isalnum((unsigned char)c)) {
                    if (token_pos < MAX_TOKEN_LEN - 1) {
                        current_token[token_pos++] = c;
                        position++;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (token_list.count == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                process_tokens(&token_list);
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        print_tokens(&token_list);
    } else {
        printf("Error: Invalid input or processing error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
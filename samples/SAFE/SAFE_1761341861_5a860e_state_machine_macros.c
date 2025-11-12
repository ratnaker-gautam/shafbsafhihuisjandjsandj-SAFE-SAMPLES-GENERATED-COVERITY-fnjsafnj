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
#define CHECK_BOUNDS(idx, max) if ((idx) >= (max)) { TRANSITION(STATE_ERROR); break; }

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int count;
} TokenList;

void initialize_tokens(TokenList *tl) {
    tl->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        memset(tl->tokens[i], 0, MAX_TOKEN_LEN);
    }
}

int add_token(TokenList *tl, const char *token, int len) {
    if (tl->count >= MAX_TOKENS || len >= MAX_TOKEN_LEN) {
        return 0;
    }
    if (len > 0) {
        strncpy(tl->tokens[tl->count], token, len);
        tl->tokens[tl->count][len] = '\0';
        tl->count++;
    }
    return 1;
}

void process_tokens(TokenList *tl) {
    printf("Processing %d tokens:\n", tl->count);
    for (int i = 0; i < tl->count; i++) {
        printf("  Token %d: '%s'\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    TokenList tokens;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int token_start = -1;
    
    printf("Enter text to tokenize (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    input_len = (int)strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return EXIT_FAILURE;
    }
    
    initialize_tokens(&tokens);
    TRANSITION(STATE_READING);
    
    for (int i = 0; i <= input_len; i++) {
        switch (current_state) {
            case STATE_READING:
                CHECK_BOUNDS(i, input_len + 1);
                if (i == input_len || input[i] == ' ') {
                    if (token_start != -1) {
                        int token_len = i - token_start;
                        if (!add_token(&tokens, &input[token_start], token_len)) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        token_start = -1;
                    }
                    if (i == input_len) {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else {
                    if (!IS_VALID_CHAR(input[i])) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    if (token_start == -1) {
                        token_start = i;
                    }
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
                
            case STATE_DONE:
                printf("Tokenization completed successfully\n");
                return EXIT_SUCCESS;
                
            case STATE_ERROR:
                printf("Error during tokenization\n");
                return EXIT_FAILURE;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state != STATE_DONE) {
        printf("Unexpected termination\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
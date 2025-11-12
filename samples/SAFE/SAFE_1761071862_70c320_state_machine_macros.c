//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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

int extract_tokens(const char* input, TokenList* tl) {
    if (input == NULL || tl == NULL) return 0;
    
    int token_count = 0;
    size_t input_len = strlen(input);
    size_t token_pos = 0;
    
    for (size_t i = 0; i <= input_len && token_count < MAX_TOKENS; i++) {
        char c = input[i];
        
        if (c == ' ' || c == ',' || c == '.' || c == '\0') {
            if (token_pos > 0) {
                if (token_pos < MAX_TOKEN_LEN) {
                    tl->tokens[token_count][token_pos] = '\0';
                    token_count++;
                }
                token_pos = 0;
            }
        } else if (isalnum((unsigned char)c)) {
            if (token_pos < MAX_TOKEN_LEN - 1) {
                tl->tokens[token_count][token_pos] = c;
                token_pos++;
            }
        }
    }
    
    tl->count = token_count;
    return token_count > 0;
}

void process_tokens(TokenList* tl) {
    if (tl == NULL || tl->count == 0) return;
    
    for (int i = 0; i < tl->count; i++) {
        for (int j = 0; tl->tokens[i][j] != '\0'; j++) {
            tl->tokens[i][j] = toupper((unsigned char)tl->tokens[i][j]);
        }
    }
}

void print_tokens(const TokenList* tl) {
    if (tl == NULL || tl->count == 0) return;
    
    printf("Processed tokens (%d):\n", tl->count);
    for (int i = 0; i < tl->count; i++) {
        printf("  %d: %s\n", i + 1, tl->tokens[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    int current_state = STATE_IDLE;
    TokenList tokens;
    
    printf("Enter text to tokenize (alphanumeric, spaces, commas, periods): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        if (len >= sizeof(input) - 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        
        TRANSITION(STATE_READING);
    }
    
    if (current_state == STATE_READING) {
        if (!validate_input(input)) {
            TRANSITION(STATE_ERROR);
        } else {
            initialize_tokens(&tokens);
            if (!extract_tokens(input, &tokens)) {
                TRANSITION(STATE_ERROR);
            } else {
                TRANSITION(STATE_PROCESSING);
            }
        }
    }
    
    if (current_state == STATE_PROCESSING) {
        process_tokens(&tokens);
        TRANSITION(STATE_DONE);
    }
    
    if (current_state == STATE_DONE) {
        print_tokens(&tokens);
        printf("Processing completed successfully.\n");
    } else if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing failed.\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
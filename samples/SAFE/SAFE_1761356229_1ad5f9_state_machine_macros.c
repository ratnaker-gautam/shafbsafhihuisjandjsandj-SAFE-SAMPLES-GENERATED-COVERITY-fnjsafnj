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

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) (isalnum((unsigned char)c) || (c) == ' ' || (c) == ',' || (c) == '.')

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char tokens[MAX_TOKENS][MAX_INPUT_LEN + 1];
    int token_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int token_pos = 0;
    int i = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    input_len = (int)strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return EXIT_SUCCESS;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    TRANSITION(STATE_READING);
    
    for (i = 0; i < input_len && current_state != STATE_ERROR && current_state != STATE_DONE; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_READING:
                if (!IS_VALID_CHAR(c)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (token_count >= MAX_TOKENS) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (isalnum((unsigned char)c)) {
                    if (token_pos < MAX_INPUT_LEN) {
                        tokens[token_count][token_pos++] = c;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else if (c == ' ' || c == ',' || c == '.') {
                    if (token_pos > 0) {
                        tokens[token_count][token_pos] = '\0';
                        token_count++;
                        token_pos = 0;
                    }
                    
                    if (c == '.') {
                        TRANSITION(STATE_PROCESSING);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (token_pos > 0) {
                    tokens[token_count][token_pos] = '\0';
                    token_count++;
                    token_pos = 0;
                }
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING && token_pos > 0) {
        tokens[token_count][token_pos] = '\0';
        token_count++;
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (token_count == 0) {
        printf("No valid tokens found\n");
        return EXIT_SUCCESS;
    }
    
    printf("Found %d token(s):\n", token_count);
    for (i = 0; i < token_count; i++) {
        if (strlen(tokens[i]) > 0) {
            printf("%d: '%s'\n", i + 1, tokens[i]);
        }
    }
    
    return EXIT_SUCCESS;
}
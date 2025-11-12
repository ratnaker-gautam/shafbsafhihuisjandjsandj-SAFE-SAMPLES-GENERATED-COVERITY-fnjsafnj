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
#define IS_DIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define IS_ALPHA(ch) (((ch) >= 'a' && (ch) <= 'z') || ((ch) >= 'A' && (ch) <= 'Z'))

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
        TRANSITION(STATE_ERROR);
    } else {
        input_len = (int)strlen(input);
        if (input_len > 0 && input[input_len - 1] == '\n') {
            input[input_len - 1] = '\0';
            input_len--;
        }
        
        if (input_len > MAX_INPUT_LEN) {
            TRANSITION(STATE_ERROR);
        } else {
            TRANSITION(STATE_READING);
        }
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (i >= input_len) {
                    if (token_pos > 0 && token_count < MAX_TOKENS) {
                        tokens[token_count][token_pos] = '\0';
                        token_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                } else if (token_count >= MAX_TOKENS) {
                    TRANSITION(STATE_ERROR);
                } else {
                    char ch = input[i];
                    if (IS_ALPHA(ch) || IS_DIGIT(ch)) {
                        if (token_pos < MAX_INPUT_LEN) {
                            tokens[token_count][token_pos] = ch;
                            token_pos++;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else if (ch == ' ' || ch == '\t') {
                        if (token_pos > 0) {
                            tokens[token_count][token_pos] = '\0';
                            token_count++;
                            token_pos = 0;
                        }
                        i++;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                    i++;
                }
                break;
                
            case STATE_PROCESSING:
                printf("Found %d tokens:\n", token_count);
                for (int j = 0; j < token_count; j++) {
                    int has_digit = 0;
                    int has_alpha = 0;
                    
                    for (int k = 0; tokens[j][k] != '\0'; k++) {
                        if (IS_DIGIT(tokens[j][k])) {
                            has_digit = 1;
                        }
                        if (IS_ALPHA(tokens[j][k])) {
                            has_alpha = 1;
                        }
                    }
                    
                    printf("Token %d: '%s' - ", j + 1, tokens[j]);
                    if (has_digit && has_alpha) {
                        printf("Mixed\n");
                    } else if (has_digit) {
                        printf("Numbers\n");
                    } else if (has_alpha) {
                        printf("Letters\n");
                    } else {
                        printf("Unknown\n");
                    }
                }
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
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
    
    TRANSITION(STATE_READING);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                for (i = 0; i < input_len; i++) {
                    if (!IS_VALID_CHAR(input[i])) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (input[i] == ' ' || input[i] == ',') {
                        if (token_pos > 0) {
                            if (token_count < MAX_TOKENS) {
                                tokens[token_count][token_pos] = '\0';
                                token_count++;
                                token_pos = 0;
                            } else {
                                TRANSITION(STATE_ERROR);
                                break;
                            }
                        }
                    } else {
                        if (token_pos < MAX_INPUT_LEN) {
                            tokens[token_count][token_pos] = input[i];
                            token_pos++;
                        } else {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                    }
                }
                
                if (current_state == STATE_READING) {
                    if (token_pos > 0 && token_count < MAX_TOKENS) {
                        tokens[token_count][token_pos] = '\0';
                        token_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                printf("Processing %d tokens:\n", token_count);
                for (i = 0; i < token_count; i++) {
                    printf("Token %d: '%s'\n", i + 1, tokens[i]);
                }
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or too many tokens\n");
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return (current_state == STATE_DONE) ? EXIT_SUCCESS : EXIT_FAILURE;
}
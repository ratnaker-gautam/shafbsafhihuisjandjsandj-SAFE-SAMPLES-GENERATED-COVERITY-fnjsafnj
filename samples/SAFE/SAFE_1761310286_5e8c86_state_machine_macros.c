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
#define IS_VALID_CHAR(c) (isalnum((unsigned char)c) || (c) == ' ' || (c) == ',')

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    char tokens[MAX_TOKENS][MAX_INPUT_LEN];
    int token_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int pos = 0;
    
    printf("Enter up to %d alphanumeric tokens separated by commas: ", MAX_TOKENS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        TRANSITION(STATE_READING);
    }
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_READING:
                if (input_len >= MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                token_count = 0;
                pos = 0;
                
                while (pos < input_len && token_count < MAX_TOKENS) {
                    while (pos < input_len && input_buffer[pos] == ' ') {
                        pos++;
                    }
                    
                    if (pos >= input_len) break;
                    
                    int token_start = pos;
                    while (pos < input_len && input_buffer[pos] != ',' && input_buffer[pos] != ' ') {
                        if (!IS_VALID_CHAR(input_buffer[pos])) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        pos++;
                    }
                    
                    if (current_state == STATE_ERROR) break;
                    
                    int token_len = pos - token_start;
                    if (token_len > 0) {
                        if (token_len >= MAX_INPUT_LEN) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        strncpy(tokens[token_count], &input_buffer[token_start], token_len);
                        tokens[token_count][token_len] = '\0';
                        token_count++;
                    }
                    
                    if (pos < input_len && input_buffer[pos] == ',') {
                        pos++;
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (token_count == 0) {
                    printf("No valid tokens found.\n");
                } else {
                    printf("Processing %d tokens:\n", token_count);
                    for (int i = 0; i < token_count; i++) {
                        if (strlen(tokens[i]) > 0) {
                            printf("Token %d: %s\n", i + 1, tokens[i]);
                        }
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
        printf("Error: Invalid input or processing error.\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
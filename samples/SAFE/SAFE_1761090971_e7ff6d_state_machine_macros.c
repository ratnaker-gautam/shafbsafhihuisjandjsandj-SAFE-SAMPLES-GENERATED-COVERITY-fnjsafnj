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

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) (isalnum((unsigned char)c) || (c) == ' ' || (c) == ',' || (c) == '.')

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    char tokens[MAX_TOKENS][MAX_INPUT_LEN + 1];
    int token_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int token_pos = 0;
    int i;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        TRANSITION(STATE_ERROR);
    } else {
        input_len = (int)strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        TRANSITION(STATE_READING);
    }
    
    for (i = 0; i < MAX_INPUT_LEN && current_state != STATE_ERROR && current_state != STATE_DONE; i++) {
        switch (current_state) {
            case STATE_READING:
                if (i >= input_len) {
                    if (token_pos > 0) {
                        tokens[token_count][token_pos] = '\0';
                        token_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                } else if (!IS_VALID_CHAR(input_buffer[i])) {
                    TRANSITION(STATE_ERROR);
                } else if (isspace((unsigned char)input_buffer[i])) {
                    if (token_pos > 0) {
                        tokens[token_count][token_pos] = '\0';
                        token_count++;
                        token_pos = 0;
                        if (token_count >= MAX_TOKENS) {
                            TRANSITION(STATE_PROCESSING);
                        }
                    }
                } else {
                    if (token_pos < MAX_INPUT_LEN) {
                        tokens[token_count][token_pos] = input_buffer[i];
                        token_pos++;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (token_count == 0) {
                    printf("No valid tokens found.\n");
                } else {
                    printf("Found %d token(s):\n", token_count);
                    for (int j = 0; j < token_count; j++) {
                        if (strlen(tokens[j]) > 0) {
                            printf("  %d: '%s'\n", j + 1, tokens[j]);
                        }
                    }
                }
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input character or buffer overflow.\n");
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_READING) {
        TRANSITION(STATE_PROCESSING);
    }
    
    return (current_state == STATE_ERROR) ? EXIT_FAILURE : EXIT_SUCCESS;
}
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
    char input_buffer[MAX_INPUT_LEN + 1];
    char tokens[MAX_TOKENS][MAX_INPUT_LEN + 1];
    int token_count = 0;
    int current_state = STATE_IDLE;
    int input_len = 0;
    int token_pos = 0;
    char ch;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    input_len = (int)strlen(input_buffer);
                    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                        input_buffer[input_len - 1] = '\0';
                        input_len--;
                    }
                    if (input_len > MAX_INPUT_LEN) {
                        TRANSITION(STATE_ERROR);
                    } else {
                        TRANSITION(STATE_READING);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                token_count = 0;
                token_pos = 0;
                for (int i = 0; i < input_len && token_count < MAX_TOKENS; i++) {
                    ch = input_buffer[i];
                    if (!IS_VALID_CHAR(ch)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (ch == ' ' || ch == ',' || ch == '.') {
                        if (token_pos > 0) {
                            tokens[token_count][token_pos] = '\0';
                            token_count++;
                            token_pos = 0;
                        }
                    } else {
                        if (token_pos < MAX_INPUT_LEN) {
                            tokens[token_count][token_pos++] = ch;
                        } else {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    if (token_pos > 0 && token_count < MAX_TOKENS) {
                        tokens[token_count][token_pos] = '\0';
                        token_count++;
                    }
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                printf("Processing %d tokens:\n", token_count);
                for (int i = 0; i < token_count; i++) {
                    printf("Token %d: %s\n", i + 1, tokens[i]);
                }
                TRANSITION(STATE_DONE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or processing error\n");
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
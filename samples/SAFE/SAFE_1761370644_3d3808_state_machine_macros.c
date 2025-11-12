//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_IDLE 0
#define STATE_READING 1
#define STATE_PROCESSING 2
#define STATE_WRITING 3
#define STATE_ERROR 4

#define MAX_INPUT_LEN 100
#define MAX_OUTPUT_LEN 200

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    int input_len = 0;
    int processed_chars = 0;
    int valid_transition = 1;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR && valid_transition) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, MAX_INPUT_LEN + 1, stdin) != NULL) {
                    input_len = (int)strlen(input_buffer);
                    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                        input_buffer[input_len - 1] = '\0';
                        input_len--;
                    }
                    if (CHECK_BOUNDS(input_len, MAX_INPUT_LEN + 1)) {
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (input_len > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (processed_chars < input_len) {
                    char current_char = input_buffer[processed_chars];
                    if (current_char >= 'a' && current_char <= 'z') {
                        output_buffer[processed_chars] = current_char - 32;
                    } else if (current_char >= 'A' && current_char <= 'Z') {
                        output_buffer[processed_chars] = current_char + 32;
                    } else {
                        output_buffer[processed_chars] = current_char;
                    }
                    processed_chars++;
                    
                    if (processed_chars >= input_len) {
                        output_buffer[processed_chars] = '\0';
                        TRANSITION(STATE_WRITING);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                if (CHECK_BOUNDS(processed_chars, MAX_OUTPUT_LEN + 1)) {
                    printf("Processed text: %s\n", output_buffer);
                    TRANSITION(STATE_IDLE);
                    processed_chars = 0;
                    input_len = 0;
                    memset(input_buffer, 0, sizeof(input_buffer));
                    memset(output_buffer, 0, sizeof(output_buffer));
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid state transition or input\n");
                valid_transition = 0;
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
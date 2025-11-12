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
    char input_buffer[MAX_INPUT_LEN] = {0};
    char output_buffer[MAX_OUTPUT_LEN] = {0};
    int input_len = 0;
    int processed_chars = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, MAX_INPUT_LEN, stdin) != NULL) {
                    input_len = (int)strnlen(input_buffer, MAX_INPUT_LEN);
                    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                        input_buffer[input_len - 1] = '\0';
                        input_len--;
                    }
                    if (CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
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
                    processed_chars = 0;
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (processed_chars < input_len) {
                    char current_char = input_buffer[processed_chars];
                    int output_pos = processed_chars * 2;
                    
                    if (CHECK_BOUNDS(output_pos + 1, MAX_OUTPUT_LEN)) {
                        if (current_char >= 'a' && current_char <= 'z') {
                            output_buffer[output_pos] = current_char - 32;
                        } else if (current_char >= 'A' && current_char <= 'Z') {
                            output_buffer[output_pos] = current_char + 32;
                        } else {
                            output_buffer[output_pos] = current_char;
                        }
                        output_buffer[output_pos + 1] = ' ';
                        processed_chars++;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    if (processed_chars * 2 < MAX_OUTPUT_LEN) {
                        output_buffer[processed_chars * 2] = '\0';
                    }
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed output: %s\n", output_buffer);
                printf("Processing complete.\n");
                return 0;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    fprintf(stderr, "Error: Invalid state or input\n");
    return 1;
}
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
#define MAX_BUFFER 50

#define TRANSITION(new_state) current_state = new_state
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ')

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    char process_buffer[MAX_BUFFER + 1];
    size_t input_len = 0;
    size_t process_index = 0;
    int input_valid = 1;
    
    printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
    
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
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_READING:
                if (input_len == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (size_t i = 0; i < input_len && process_index < MAX_BUFFER; i++) {
                    if (!IS_VALID_CHAR(input_buffer[i])) {
                        input_valid = 0;
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (input_buffer[i] != ' ') {
                        if (process_index < MAX_BUFFER) {
                            process_buffer[process_index++] = input_buffer[i];
                        } else {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                    }
                    
                    if (i == input_len - 1) {
                        TRANSITION(STATE_PROCESSING);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (process_index == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (size_t i = 0; i < process_index; i++) {
                    if (process_buffer[i] >= 'a' && process_buffer[i] <= 'z') {
                        process_buffer[i] = process_buffer[i] - 'a' + 'A';
                    }
                }
                process_buffer[process_index] = '\0';
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                if (process_index > 0 && process_buffer[0] != '\0') {
                    printf("Processed result: %s\n", process_buffer);
                    printf("Characters processed: %zu\n", process_index);
                } else {
                    printf("No valid characters to process\n");
                }
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or processing error\n");
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return input_valid ? EXIT_SUCCESS : EXIT_FAILURE;
}
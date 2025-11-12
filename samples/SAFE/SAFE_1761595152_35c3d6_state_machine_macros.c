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
#define MAX_BUFFER_SIZE 50

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ')

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    char process_buffer[MAX_BUFFER_SIZE + 1];
    size_t input_len = 0;
    size_t buffer_pos = 0;
    int result = 0;

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

    for (size_t i = 0; i <= input_len && current_state != STATE_ERROR; i++) {
        switch (current_state) {
            case STATE_READING:
                if (i < input_len) {
                    char current_char = input_buffer[i];
                    if (IS_VALID_CHAR(current_char)) {
                        if (buffer_pos < MAX_BUFFER_SIZE) {
                            process_buffer[buffer_pos++] = current_char;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    } else if (current_char != '\0') {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    if (buffer_pos > 0) {
                        process_buffer[buffer_pos] = '\0';
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;

            case STATE_PROCESSING:
                if (buffer_pos > 0) {
                    for (size_t j = 0; j < buffer_pos; j++) {
                        if (process_buffer[j] >= 'a' && process_buffer[j] <= 'z') {
                            process_buffer[j] = process_buffer[j] - 'a' + 'A';
                        }
                    }
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;

            case STATE_WRITING:
                printf("Processed text: %s\n", process_buffer);
                result = 1;
                TRANSITION(STATE_IDLE);
                break;

            case STATE_ERROR:
                printf("Error: Invalid input or processing error\n");
                result = 0;
                break;

            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }

    if (current_state == STATE_READING && buffer_pos > 0) {
        process_buffer[buffer_pos] = '\0';
        for (size_t j = 0; j < buffer_pos; j++) {
            if (process_buffer[j] >= 'a' && process_buffer[j] <= 'z') {
                process_buffer[j] = process_buffer[j] - 'a' + 'A';
            }
        }
        printf("Processed text: %s\n", process_buffer);
        result = 1;
    }

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
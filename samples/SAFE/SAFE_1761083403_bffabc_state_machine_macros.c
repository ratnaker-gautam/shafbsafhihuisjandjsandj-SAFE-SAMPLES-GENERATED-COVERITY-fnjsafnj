//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

static int process_data(const char* input, char* output, size_t len) {
    if (input == NULL || output == NULL || len == 0 || len > MAX_INPUT_LEN) {
        return -1;
    }
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) {
            return -1;
        }
        if (out_idx >= MAX_OUTPUT_LEN - 2) {
            return -1;
        }
        output[out_idx++] = input[i];
        if (i % 3 == 2 && out_idx < MAX_OUTPUT_LEN - 2) {
            output[out_idx++] = '-';
        }
    }
    output[out_idx] = '\0';
    return 0;
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    size_t input_length = 0;
    
    printf("Enter data (max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR && current_state != STATE_WRITING) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING: {
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_length = strlen(input_buffer);
                if (input_length > 0 && input_buffer[input_length - 1] == '\n') {
                    input_buffer[--input_length] = '\0';
                }
                
                if (input_length == 0) {
                    printf("Empty input. Try again: ");
                    TRANSITION(STATE_READING);
                } else if (input_length > MAX_INPUT_LEN) {
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
            }
                
            case STATE_PROCESSING:
                if (process_data(input_buffer, output_buffer, input_length) == 0) {
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed: %s\n", output_buffer);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input or processing failed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
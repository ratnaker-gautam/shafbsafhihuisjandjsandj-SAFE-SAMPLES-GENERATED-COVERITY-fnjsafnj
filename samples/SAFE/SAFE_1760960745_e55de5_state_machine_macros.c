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

static int process_input(const char* input, char* output, size_t output_size) {
    if (input == NULL || output == NULL || output_size == 0) {
        return -1;
    }
    
    size_t input_len = strnlen(input, MAX_INPUT_LEN);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    size_t output_idx = 0;
    for (size_t i = 0; i < input_len && output_idx < output_size - 1; i++) {
        if (IS_VALID_CHAR(input[i])) {
            output[output_idx++] = input[i];
        }
    }
    
    if (output_idx == 0) {
        return -1;
    }
    
    output[output_idx] = '\0';
    return 0;
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    int retval = EXIT_SUCCESS;
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN);
                fflush(stdout);
                
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                size_t len = strnlen(input_buffer, sizeof(input_buffer));
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0';
                }
                
                if (strnlen(input_buffer, sizeof(input_buffer)) == 0) {
                    printf("Empty input, please try again.\n");
                    break;
                }
                
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                printf("Read: '%s'\n", input_buffer);
                TRANSITION(STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING:
                if (process_input(input_buffer, output_buffer, sizeof(output_buffer)) != 0) {
                    printf("Error: Failed to process input\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                printf("Processing completed.\n");
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Output: '%s'\n", output_buffer);
                printf("Continue? (y/n): ");
                fflush(stdout);
                
                char response[16] = {0};
                if (fgets(response, sizeof(response), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (response[0] == 'y' || response[0] == 'Y') {
                    memset(input_buffer, 0, sizeof(input_buffer));
                    memset(output_buffer, 0, sizeof(output_buffer));
                    TRANSITION(STATE_IDLE);
                } else if (response[0] == 'n' || response[0] == 'N') {
                    printf("Exiting.\n");
                    retval = EXIT_SUCCESS;
                    current_state = STATE_ERROR;
                } else {
                    printf("Invalid response, please enter 'y' or 'n'\n");
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        retval = EXIT_FAILURE;
    }
    
    return retval;
}
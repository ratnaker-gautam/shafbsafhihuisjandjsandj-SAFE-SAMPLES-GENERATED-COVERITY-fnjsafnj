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
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

static int process_input(const char* input, char* output, size_t output_size) {
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return -1;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return -1;
        if (out_idx >= output_size - 1) return -1;
        
        if (input[i] >= 'a' && input[i] <= 'z') {
            output[out_idx++] = input[i] - 32;
        } else if (input[i] >= 'A' && input[i] <= 'Z') {
            output[out_idx++] = input[i] + 32;
        } else {
            output[out_idx++] = input[i];
        }
    }
    output[out_idx] = '\0';
    return 0;
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    int result = 0;
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                size_t len = strlen(input_buffer);
                if (len > 0 && input_buffer[len - 1] == '\n') {
                    input_buffer[len - 1] = '\0';
                }
                
                if (strlen(input_buffer) == 0) {
                    printf("Empty input, staying in idle state\n");
                    break;
                }
                
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                if (strlen(input_buffer) >= MAX_INPUT_LEN) {
                    printf("Input too long\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (size_t i = 0; i < strlen(input_buffer); i++) {
                    if (!IS_VALID_CHAR(input_buffer[i])) {
                        printf("Invalid character in input\n");
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                
                TRANSITION(STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING:
                result = process_input(input_buffer, output_buffer, sizeof(output_buffer));
                if (result != 0) {
                    printf("Processing failed\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed result: %s\n", output_buffer);
                printf("Continue? (y/n): ");
                
                char response[10];
                if (fgets(response, sizeof(response), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (response[0] == 'y' || response[0] == 'Y') {
                    memset(input_buffer, 0, sizeof(input_buffer));
                    memset(output_buffer, 0, sizeof(output_buffer));
                    TRANSITION(STATE_IDLE);
                } else if (response[0] == 'n' || response[0] == 'N') {
                    printf("Exiting state machine\n");
                    return 0;
                } else {
                    printf("Invalid response, staying in writing state\n");
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf("State machine entered error state\n");
    return 1;
}
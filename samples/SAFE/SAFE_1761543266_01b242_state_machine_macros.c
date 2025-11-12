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

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    size_t input_len = 0;
    int processing_count = 0;
    
    printf("State Machine Demo - Enter text lines (empty to quit):\n");
    
    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                printf("IDLE> ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_len = strlen(input_buffer);
                if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                    input_buffer[input_len - 1] = '\0';
                    input_len--;
                }
                
                if (input_len == 0) {
                    printf("Empty input - terminating\n");
                    TRANSITION(STATE_ERROR);
                } else if (input_len >= MAX_INPUT_LEN) {
                    printf("Input too long\n");
                    TRANSITION(STATE_ERROR);
                } else {
                    TRANSITION(STATE_READING);
                }
                break;
                
            case STATE_READING:
                printf("READ: '%s'\n", input_buffer);
                TRANSITION(STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING:
                processing_count++;
                if (processing_count > 10) {
                    printf("Processing limit reached\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                size_t output_len = 0;
                for (size_t i = 0; i < input_len && output_len < MAX_OUTPUT_LEN - 3; i++) {
                    if (input_buffer[i] >= 'a' && input_buffer[i] <= 'z') {
                        output_buffer[output_len++] = input_buffer[i] - 32;
                    } else if (input_buffer[i] >= 'A' && input_buffer[i] <= 'Z') {
                        output_buffer[output_len++] = input_buffer[i] + 32;
                    } else {
                        output_buffer[output_len++] = input_buffer[i];
                    }
                }
                output_buffer[output_len] = '\0';
                
                printf("PROCESSED (%d): '%s'\n", processing_count, output_buffer);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("WRITTEN: '%s'\n", output_buffer);
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(output_buffer, 0, sizeof(output_buffer));
                input_len = 0;
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("ERROR state reached - cleaning up\n");
                return 1;
                
            default:
                printf("Invalid state\n");
                return 1;
        }
    }
    
    return 0;
}
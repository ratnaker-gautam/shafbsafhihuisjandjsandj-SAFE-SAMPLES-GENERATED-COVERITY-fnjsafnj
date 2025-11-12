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
                    input_len = (int)strlen(input_buffer);
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
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (processed_chars < input_len) {
                    char current_char = input_buffer[processed_chars];
                    if (CHECK_BOUNDS(processed_chars * 2, MAX_OUTPUT_LEN - 2)) {
                        output_buffer[processed_chars * 2] = current_char;
                        output_buffer[processed_chars * 2 + 1] = current_char;
                        processed_chars++;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    if (CHECK_BOUNDS(processed_chars * 2, MAX_OUTPUT_LEN)) {
                        output_buffer[processed_chars * 2] = '\0';
                        TRANSITION(STATE_WRITING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_WRITING:
                printf("Processed output: %s\n", output_buffer);
                TRANSITION(STATE_IDLE);
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(output_buffer, 0, sizeof(output_buffer));
                input_len = 0;
                processed_chars = 0;
                printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid state transition or buffer overflow\n");
                return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}
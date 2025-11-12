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
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    int input_len = 0;
    int process_len = 0;
    int ret;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
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
                if (CHECK_BOUNDS(input_len, MAX_BUFFER_SIZE)) {
                    strncpy(process_buffer, input_buffer, input_len);
                    process_buffer[input_len] = '\0';
                    process_len = input_len;
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (process_len > 0 && CHECK_BOUNDS(process_len, MAX_BUFFER_SIZE)) {
                    for (int i = 0; i < process_len; i++) {
                        if (process_buffer[i] >= 'a' && process_buffer[i] <= 'z') {
                            process_buffer[i] = process_buffer[i] - 'a' + 'A';
                        }
                    }
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                if (CHECK_BOUNDS(process_len, MAX_BUFFER_SIZE)) {
                    ret = printf("Processed: %s\n", process_buffer);
                    if (ret > 0) {
                        printf("State machine completed successfully.\n");
                        return EXIT_SUCCESS;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    fprintf(stderr, "State machine error occurred.\n");
    return EXIT_FAILURE;
}
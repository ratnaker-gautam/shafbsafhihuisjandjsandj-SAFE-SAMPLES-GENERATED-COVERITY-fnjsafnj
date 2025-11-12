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
#define STATE_DONE 5

#define MAX_INPUT_LEN 100
#define MAX_BUFFER_SIZE 50

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))
#define IS_PRINTABLE(c) ((c) >= 32 && (c) <= 126)

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    int input_len = 0;
    int process_len = 0;
    int i;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
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
                process_len = 0;
                for (i = 0; i < input_len && process_len < MAX_BUFFER_SIZE; i++) {
                    if (IS_PRINTABLE(input_buffer[i])) {
                        process_buffer[process_len++] = input_buffer[i];
                    }
                }
                process_buffer[process_len] = '\0';
                if (process_len > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                for (i = 0; i < process_len / 2; i++) {
                    char temp = process_buffer[i];
                    process_buffer[i] = process_buffer[process_len - 1 - i];
                    process_buffer[process_len - 1 - i] = temp;
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed result: %s\n", process_buffer);
                printf("Continue? (y/n): ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    if (input_buffer[0] == 'y' || input_buffer[0] == 'Y') {
                        TRANSITION(STATE_IDLE);
                    } else if (input_buffer[0] == 'n' || input_buffer[0] == 'N') {
                        TRANSITION(STATE_DONE);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_ERROR:
                printf("Error occurred in processing\n");
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf("Program completed\n");
    return 0;
}
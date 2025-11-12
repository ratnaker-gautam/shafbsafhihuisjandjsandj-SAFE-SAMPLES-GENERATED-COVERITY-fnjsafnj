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
    char input_buffer[MAX_INPUT_LEN] = {0};
    char process_buffer[MAX_BUFFER_SIZE] = {0};
    int input_len = 0;
    int process_len = 0;
    char input_char;
    
    printf("State Machine Demo - Enter characters (q to quit):\n");
    
    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                printf("IDLE: Ready for input\n");
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_len = strnlen(input_buffer, MAX_INPUT_LEN);
                if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                    input_buffer[input_len - 1] = '\0';
                    input_len--;
                }
                
                if (input_len == 1 && input_buffer[0] == 'q') {
                    printf("Quitting...\n");
                    return 0;
                }
                
                if (input_len > 0) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_PROCESSING:
                process_len = 0;
                for (int i = 0; i < input_len && process_len < MAX_BUFFER_SIZE - 1; i++) {
                    char c = input_buffer[i];
                    if (c >= 'a' && c <= 'z') {
                        process_buffer[process_len++] = c - 32;
                    } else if (c >= 'A' && c <= 'Z') {
                        process_buffer[process_len++] = c + 32;
                    } else if (c >= '0' && c <= '9') {
                        process_buffer[process_len++] = c;
                    }
                }
                process_buffer[process_len] = '\0';
                
                if (process_len > 0) {
                    TRANSITION(STATE_WRITING);
                } else {
                    printf("No valid characters to process\n");
                    TRANSITION(STATE_IDLE);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed: %s\n", process_buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error occurred - resetting\n");
                memset(input_buffer, 0, MAX_INPUT_LEN);
                memset(process_buffer, 0, MAX_BUFFER_SIZE);
                input_len = 0;
                process_len = 0;
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return 0;
}
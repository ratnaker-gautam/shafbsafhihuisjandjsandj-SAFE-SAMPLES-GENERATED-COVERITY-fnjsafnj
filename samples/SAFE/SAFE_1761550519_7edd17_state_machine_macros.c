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
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    int input_len = 0;
    int process_len = 0;
    int processed_count = 0;
    char ch;
    
    printf("Enter data (max %d chars, 'q' to quit): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (scanf("%c", &ch) != 1) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (ch == 'q') {
                    printf("Quitting...\n");
                    goto cleanup;
                }
                
                if (input_len < MAX_INPUT_LEN) {
                    input_buffer[input_len++] = ch;
                    TRANSITION(STATE_READING);
                } else {
                    printf("Input buffer full\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (scanf("%c", &ch) != 1) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (ch == '\n') {
                    input_buffer[input_len] = '\0';
                    TRANSITION(STATE_PROCESSING);
                } else if (ch == 'q') {
                    printf("Quitting...\n");
                    goto cleanup;
                } else if (input_len < MAX_INPUT_LEN) {
                    input_buffer[input_len++] = ch;
                } else {
                    printf("Input buffer full\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                process_len = 0;
                for (int i = 0; i < input_len; i++) {
                    if (!CHECK_BOUNDS(process_len, MAX_BUFFER_SIZE)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (IS_DIGIT(input_buffer[i])) {
                        process_buffer[process_len++] = input_buffer[i];
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    process_buffer[process_len] = '\0';
                    processed_count++;
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                if (process_len > 0) {
                    printf("Processed %d: Extracted digits: %s\n", processed_count, process_buffer);
                } else {
                    printf("Processed %d: No digits found\n", processed_count);
                }
                
                input_len = 0;
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(process_buffer, 0, sizeof(process_buffer));
                printf("Enter more data (max %d chars, 'q' to quit): ", MAX_INPUT_LEN);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error occurred in state machine\n");
                goto cleanup;
                break;
        }
    }

cleanup:
    printf("State machine terminated. Processed %d inputs.\n", processed_count);
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
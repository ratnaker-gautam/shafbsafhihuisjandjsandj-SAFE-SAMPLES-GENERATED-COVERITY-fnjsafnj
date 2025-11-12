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
#define IS_VALID_CHAR(c) ((c) >= 'a' && (c) <= 'z')
#define TO_UPPER(c) ((c) - 32)

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    char process_buffer[MAX_BUFFER_SIZE + 1];
    size_t input_len = 0;
    size_t process_index = 0;
    int c;
    
    printf("Enter text (a-z only, max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                c = getchar();
                if (c == EOF || c == '\n') {
                    if (input_len == 0) {
                        printf("No input received\n");
                        TRANSITION(STATE_ERROR);
                    } else {
                        input_buffer[input_len] = '\0';
                        TRANSITION(STATE_PROCESSING);
                    }
                } else if (input_len >= MAX_INPUT_LEN) {
                    printf("Input too long\n");
                    TRANSITION(STATE_ERROR);
                } else if (!IS_VALID_CHAR(c)) {
                    printf("Invalid character: %c\n", c);
                    TRANSITION(STATE_ERROR);
                } else {
                    input_buffer[input_len++] = (char)c;
                }
                break;
                
            case STATE_PROCESSING:
                if (process_index >= input_len) {
                    process_buffer[process_index] = '\0';
                    TRANSITION(STATE_WRITING);
                } else if (process_index >= MAX_BUFFER_SIZE) {
                    printf("Processing buffer overflow\n");
                    TRANSITION(STATE_ERROR);
                } else {
                    char current_char = input_buffer[process_index];
                    process_buffer[process_index] = TO_UPPER(current_char);
                    process_index++;
                }
                break;
                
            case STATE_WRITING:
                printf("Processed result: %s\n", process_buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("State machine encountered an error\n");
                return EXIT_FAILURE;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
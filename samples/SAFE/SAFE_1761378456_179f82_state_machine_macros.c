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
#define MAX_BUFFER 256

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define CHECK_BOUNDS(len, max) ((len) < (max))

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER] = {0};
    size_t input_len = 0;
    int ch;
    int result;
    
    printf("Enter text (max %d chars, Ctrl+D to end): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR && current_state != STATE_IDLE) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                ch = getchar();
                if (ch == EOF) {
                    if (input_len == 0) {
                        printf("No input provided.\n");
                        TRANSITION(STATE_ERROR);
                    } else {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else if (ch == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
                        printf("Input too long.\n");
                        TRANSITION(STATE_ERROR);
                    } else if (!IS_VALID_CHAR(ch)) {
                        printf("Invalid character detected.\n");
                        TRANSITION(STATE_ERROR);
                    } else {
                        input_buffer[input_len++] = (char)ch;
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (input_len == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_buffer[input_len] = '\0';
                
                size_t process_len = 0;
                for (size_t i = 0; i < input_len; i++) {
                    if (!CHECK_BOUNDS(process_len, MAX_BUFFER - 1)) {
                        printf("Processing buffer overflow.\n");
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    char current = input_buffer[i];
                    if (current >= 'a' && current <= 'z') {
                        process_buffer[process_len++] = current - 32;
                    } else if (current >= 'A' && current <= 'Z') {
                        process_buffer[process_len++] = current + 32;
                    } else {
                        process_buffer[process_len++] = current;
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    process_buffer[process_len] = '\0';
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                printf("Original: %s\n", input_buffer);
                printf("Processed: %s\n", process_buffer);
                printf("Input length: %zu\n", input_len);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("State machine encountered an error.\n");
                return EXIT_FAILURE;
        }
    }
    
    if (current_state == STATE_IDLE && input_len == 0) {
        printf("No processing performed.\n");
    }
    
    return EXIT_SUCCESS;
}
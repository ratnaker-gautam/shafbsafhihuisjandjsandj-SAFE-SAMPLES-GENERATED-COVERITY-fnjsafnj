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
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    size_t input_pos = 0;
    size_t process_pos = 0;
    int ch;
    int digit_sum = 0;
    
    printf("Enter text (max %d chars, Ctrl+D to end): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR && (ch = getchar()) != EOF) {
        switch (current_state) {
            case STATE_IDLE:
                if (IS_VALID_CHAR(ch)) {
                    if (input_pos < MAX_INPUT_LEN) {
                        input_buffer[input_pos++] = (char)ch;
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_READING:
                if (ch == '\n' || ch == EOF) {
                    input_buffer[input_pos] = '\0';
                    TRANSITION(STATE_PROCESSING);
                } else if (IS_VALID_CHAR(ch)) {
                    if (input_pos < MAX_INPUT_LEN) {
                        input_buffer[input_pos++] = (char)ch;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos > 0) {
                    size_t i;
                    for (i = 0; i < input_pos && process_pos < MAX_BUFFER_SIZE; i++) {
                        if (IS_DIGIT(input_buffer[i])) {
                            process_buffer[process_pos++] = input_buffer[i];
                            digit_sum += input_buffer[i] - '0';
                        }
                    }
                    process_buffer[process_pos] = '\0';
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Original input: %s\n", input_buffer);
                printf("Extracted digits: %s\n", process_pos > 0 ? process_buffer : "(none)");
                printf("Sum of digits: %d\n", digit_sum);
                
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(process_buffer, 0, sizeof(process_buffer));
                input_pos = 0;
                process_pos = 0;
                digit_sum = 0;
                
                printf("Enter more text (max %d chars, Ctrl+D to end): ", MAX_INPUT_LEN);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return EXIT_FAILURE;
        }
    }
    
    if (current_state == STATE_READING) {
        input_buffer[input_pos] = '\0';
        TRANSITION(STATE_PROCESSING);
    }
    
    if (current_state == STATE_PROCESSING || current_state == STATE_WRITING) {
        if (current_state == STATE_PROCESSING) {
            size_t i;
            for (i = 0; i < input_pos && process_pos < MAX_BUFFER_SIZE; i++) {
                if (IS_DIGIT(input_buffer[i])) {
                    process_buffer[process_pos++] = input_buffer[i];
                    digit_sum += input_buffer[i] - '0';
                }
            }
            process_buffer[process_pos] = '\0';
        }
        
        printf("Final input: %s\n", input_buffer);
        printf("Final extracted digits: %s\n", process_pos > 0 ? process_buffer : "(none)");
        printf("Final sum of digits: %d\n", digit_sum);
    }
    
    return EXIT_SUCCESS;
}
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
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                ch = getchar();
                if (ch == EOF) {
                    TRANSITION(STATE_PROCESSING);
                } else if (ch == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else if (input_pos < MAX_INPUT_LEN && IS_VALID_CHAR(ch)) {
                    input_buffer[input_pos++] = (char)ch;
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                ch = getchar();
                if (ch == EOF || ch == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else if (input_pos < MAX_INPUT_LEN && IS_VALID_CHAR(ch)) {
                    input_buffer[input_pos++] = (char)ch;
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos > 0) {
                    for (size_t i = 0; i < input_pos; i++) {
                        if (process_pos < MAX_BUFFER_SIZE && IS_DIGIT(input_buffer[i])) {
                            process_buffer[process_pos++] = input_buffer[i];
                            digit_sum += (input_buffer[i] - '0');
                        }
                    }
                    process_buffer[process_pos] = '\0';
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Extracted digits: %s\n", process_buffer);
                printf("Digit sum: %d\n", digit_sum);
                printf("Processing complete.\n");
                return 0;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return 1;
        }
    }
    
    return 0;
}
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
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    size_t input_pos = 0;
    size_t output_pos = 0;
    int c;
    
    printf("Enter text (max %d chars, Ctrl+D to end):\n", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                c = getchar();
                if (c == EOF) {
                    if (input_pos > 0) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        printf("No input received.\n");
                        TRANSITION(STATE_ERROR);
                    }
                } else if (c == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    if (input_pos < MAX_INPUT_LEN) {
                        if (IS_VALID_CHAR(c)) {
                            input_buffer[input_pos++] = (char)c;
                        } else {
                            printf("Invalid character detected.\n");
                            TRANSITION(STATE_ERROR);
                        }
                    } else {
                        printf("Input too long.\n");
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                output_pos = 0;
                for (size_t i = 0; i < input_pos && output_pos < MAX_OUTPUT_LEN - 2; i++) {
                    char current = input_buffer[i];
                    if (current >= 'a' && current <= 'z') {
                        output_buffer[output_pos++] = current - 32;
                    } else if (current >= 'A' && current <= 'Z') {
                        output_buffer[output_pos++] = current + 32;
                    } else if (IS_VALID_CHAR(current)) {
                        output_buffer[output_pos++] = current;
                    }
                    
                    if (output_pos < MAX_OUTPUT_LEN - 1) {
                        output_buffer[output_pos++] = ' ';
                    }
                }
                
                if (output_pos > 0 && output_buffer[output_pos - 1] == ' ') {
                    output_buffer[output_pos - 1] = '\0';
                } else {
                    output_buffer[output_pos] = '\0';
                }
                
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed output: %s\n", output_buffer);
                printf("Processing complete.\n");
                TRANSITION(STATE_ERROR);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("State machine terminated.\n");
    }
    
    return 0;
}
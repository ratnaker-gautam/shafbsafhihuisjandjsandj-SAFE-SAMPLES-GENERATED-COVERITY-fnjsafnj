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
#define IS_VALID_CHAR(c) ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ')

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_BUFFER] = {0};
    size_t input_pos = 0;
    int ch;
    
    printf("Enter text (max %d chars, Ctrl+D to end): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                ch = getchar();
                if (ch == EOF) {
                    if (input_pos > 0) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        printf("No input received\n");
                        TRANSITION(STATE_ERROR);
                    }
                } else if (ch == '\n') {
                    if (input_pos > 0) {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else {
                    if (input_pos < MAX_INPUT_LEN) {
                        if (IS_VALID_CHAR(ch)) {
                            input_buffer[input_pos++] = (char)ch;
                        } else {
                            printf("Invalid character: %c\n", ch);
                            TRANSITION(STATE_ERROR);
                        }
                    } else {
                        printf("Input too long\n");
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_buffer[input_pos] = '\0';
                size_t output_len = 0;
                
                for (size_t i = 0; i < input_pos && output_len < MAX_BUFFER - 1; i++) {
                    char current = input_buffer[i];
                    if (current >= 'a' && current <= 'z') {
                        output_buffer[output_len++] = current - 32;
                    } else if (current >= 'A' && current <= 'Z') {
                        output_buffer[output_len++] = current + 32;
                    } else if (current >= '0' && current <= '9') {
                        output_buffer[output_len++] = 'X';
                    } else if (current == ' ') {
                        output_buffer[output_len++] = '_';
                    }
                }
                
                if (output_len > 0) {
                    output_buffer[output_len] = '\0';
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed output: %s\n", output_buffer);
                printf("Processing complete\n");
                return 0;
                
            case STATE_ERROR:
                printf("State machine error occurred\n");
                return 1;
        }
    }
    
    return 1;
}
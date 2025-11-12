//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: state_machine
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
    int ch;
    
    printf("Enter text (max %d chars, Ctrl+D to end):\n", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                ch = getchar();
                if (ch == EOF) {
                    if (input_pos == 0) {
                        printf("No input provided.\n");
                        TRANSITION(STATE_ERROR);
                    } else {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else if (ch == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    if (input_pos < MAX_INPUT_LEN) {
                        if (IS_VALID_CHAR(ch)) {
                            input_buffer[input_pos++] = (char)ch;
                        } else {
                            printf("Invalid character encountered.\n");
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
                
                input_buffer[input_pos] = '\0';
                output_pos = 0;
                
                for (size_t i = 0; i < input_pos; i++) {
                    if (output_pos >= MAX_OUTPUT_LEN) {
                        printf("Output buffer overflow.\n");
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    char current = input_buffer[i];
                    if (current >= 'a' && current <= 'z') {
                        output_buffer[output_pos++] = current - 32;
                    } else if (current >= 'A' && current <= 'Z') {
                        output_buffer[output_pos++] = current + 32;
                    } else if (IS_VALID_CHAR(current)) {
                        output_buffer[output_pos++] = current;
                    } else {
                        printf("Invalid character in processing.\n");
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    output_buffer[output_pos] = '\0';
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                if (output_pos > 0) {
                    printf("Processed output: %s\n", output_buffer);
                }
                printf("Processing complete.\n");
                return EXIT_SUCCESS;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    printf("State machine terminated with error.\n");
    return EXIT_FAILURE;
}
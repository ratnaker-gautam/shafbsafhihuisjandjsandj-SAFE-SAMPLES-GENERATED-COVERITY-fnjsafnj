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
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    size_t input_pos = 0;
    size_t output_pos = 0;
    int ch;
    int digit_count = 0;
    
    printf("Enter text (max %d chars, Ctrl+D to end): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                ch = getchar();
                if (ch == EOF) {
                    TRANSITION(STATE_WRITING);
                } else if (input_pos < MAX_INPUT_LEN) {
                    if (IS_VALID_CHAR(ch)) {
                        input_buffer[input_pos++] = (char)ch;
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                ch = getchar();
                if (ch == EOF || ch == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else if (input_pos < MAX_INPUT_LEN) {
                    if (IS_VALID_CHAR(ch)) {
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
                    for (size_t i = 0; i < input_pos; i++) {
                        if (output_pos >= MAX_OUTPUT_LEN - 10) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                        
                        if (IS_DIGIT(input_buffer[i])) {
                            digit_count++;
                            output_pos += (size_t)snprintf(output_buffer + output_pos, 
                                MAX_OUTPUT_LEN - output_pos, "[DIGIT:%c]", input_buffer[i]);
                        } else {
                            output_pos += (size_t)snprintf(output_buffer + output_pos, 
                                MAX_OUTPUT_LEN - output_pos, "[CHAR:%c]", input_buffer[i]);
                        }
                        
                        if (output_pos >= MAX_OUTPUT_LEN) {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                    }
                    
                    if (current_state != STATE_ERROR) {
                        TRANSITION(STATE_WRITING);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                if (output_pos > 0) {
                    printf("Processed output (%d digits found):\n", digit_count);
                    if (fwrite(output_buffer, 1, output_pos, stdout) != output_pos) {
                        TRANSITION(STATE_ERROR);
                    } else {
                        printf("\nProcessing complete.\n");
                        return EXIT_SUCCESS;
                    }
                } else {
                    printf("No input processed.\n");
                    return EXIT_SUCCESS;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid input or buffer overflow\n");
                return EXIT_FAILURE;
        }
    }
    
    return EXIT_FAILURE;
}
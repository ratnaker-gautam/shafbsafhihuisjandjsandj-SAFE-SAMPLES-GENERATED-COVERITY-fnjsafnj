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
                        printf("No input received\n");
                        TRANSITION(STATE_ERROR);
                    }
                } else if (c == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else if (input_pos < MAX_INPUT_LEN && IS_VALID_CHAR(c)) {
                    input_buffer[input_pos++] = (char)c;
                } else if (!IS_VALID_CHAR(c)) {
                    printf("Invalid character detected\n");
                    TRANSITION(STATE_ERROR);
                } else {
                    printf("Input too long\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (size_t i = 0; i < input_pos; i++) {
                    if (output_pos >= MAX_OUTPUT_LEN - 3) {
                        printf("Output buffer full\n");
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    char processed = input_buffer[i];
                    if (processed >= 'a' && processed <= 'z') {
                        processed = (char)(processed - 32);
                    } else if (processed >= 'A' && processed <= 'Z') {
                        processed = (char)(processed + 32);
                    }
                    
                    output_buffer[output_pos++] = processed;
                    
                    if (i < input_pos - 1) {
                        output_buffer[output_pos++] = '-';
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                if (output_pos > 0 && output_pos <= MAX_OUTPUT_LEN) {
                    output_buffer[output_pos] = '\0';
                    printf("Processed output: %s\n", output_buffer);
                    TRANSITION(STATE_IDLE);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("State machine terminated with error\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
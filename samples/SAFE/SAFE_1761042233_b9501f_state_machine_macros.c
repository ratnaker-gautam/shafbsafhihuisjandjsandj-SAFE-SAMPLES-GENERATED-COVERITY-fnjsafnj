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
#define MAX_BUFFER_SIZE 50

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define IS_VALID_CHAR(c) ((c) >= 'a' && (c) <= 'z')
#define TO_UPPER(c) ((c) - 32)

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_BUFFER_SIZE + 1] = {0};
    size_t input_pos = 0;
    size_t output_pos = 0;
    int c;
    
    printf("Enter text (a-z only, max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                c = getchar();
                if (c == EOF || c == '\n') {
                    if (input_pos == 0) {
                        printf("No input received\n");
                        TRANSITION(STATE_ERROR);
                    } else {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else if (input_pos >= MAX_INPUT_LEN) {
                    printf("Input too long\n");
                    TRANSITION(STATE_ERROR);
                } else if (!IS_VALID_CHAR(c)) {
                    printf("Invalid character: %c\n", c);
                    TRANSITION(STATE_ERROR);
                } else {
                    input_buffer[input_pos++] = (char)c;
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (size_t i = 0; i < input_pos; i++) {
                    if (output_pos >= MAX_BUFFER_SIZE) {
                        break;
                    }
                    output_buffer[output_pos++] = TO_UPPER(input_buffer[i]);
                }
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                if (output_pos > 0) {
                    printf("Processed output: ");
                    for (size_t i = 0; i < output_pos; i++) {
                        putchar(output_buffer[i]);
                    }
                    putchar('\n');
                }
                TRANSITION(STATE_ERROR);
                break;
                
            case STATE_ERROR:
                break;
        }
    }
    
    return current_state == STATE_WRITING ? EXIT_SUCCESS : EXIT_FAILURE;
}
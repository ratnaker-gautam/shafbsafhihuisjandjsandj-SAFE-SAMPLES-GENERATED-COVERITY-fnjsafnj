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
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    size_t input_pos = 0;
    size_t process_pos = 0;
    int c;
    
    printf("Enter text (a-z only, max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (input_pos < MAX_INPUT_LEN) {
                    c = getchar();
                    if (c == EOF || c == '\n') {
                        if (input_pos > 0) {
                            TRANSITION(STATE_PROCESSING);
                        } else {
                            printf("No input received\n");
                            TRANSITION(STATE_ERROR);
                        }
                    } else if (IS_VALID_CHAR(c)) {
                        input_buffer[input_pos++] = (char)c;
                        if (input_pos >= MAX_INPUT_LEN) {
                            TRANSITION(STATE_PROCESSING);
                        }
                    } else {
                        printf("Invalid character: %c\n", c);
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (process_pos < MAX_BUFFER_SIZE && process_pos < input_pos) {
                    char processed = TO_UPPER(input_buffer[process_pos]);
                    process_buffer[process_pos] = processed;
                    process_pos++;
                } else {
                    process_buffer[process_pos] = '\0';
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed text: %s\n", process_buffer);
                printf("Original length: %zu\n", input_pos);
                TRANSITION(STATE_ERROR);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
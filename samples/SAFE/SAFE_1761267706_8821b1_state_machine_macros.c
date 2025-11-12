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
    int input_char;
    
    printf("Enter text (a-z only, max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                input_char = getchar();
                if (input_char == EOF) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (input_char == '\n') {
                    if (input_pos > 0) {
                        input_buffer[input_pos] = '\0';
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        printf("No input received.\n");
                        TRANSITION(STATE_ERROR);
                    }
                    break;
                }
                
                if (input_pos >= MAX_INPUT_LEN) {
                    printf("Input too long.\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (!IS_VALID_CHAR(input_char)) {
                    printf("Invalid character: %c\n", input_char);
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_buffer[input_pos++] = (char)input_char;
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                input_char = getchar();
                if (input_char == EOF) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (input_char == '\n') {
                    input_buffer[input_pos] = '\0';
                    TRANSITION(STATE_PROCESSING);
                    break;
                }
                
                if (input_pos >= MAX_INPUT_LEN) {
                    printf("Input too long.\n");
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                if (!IS_VALID_CHAR(input_char)) {
                    printf("Invalid character: %c\n", input_char);
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_buffer[input_pos++] = (char)input_char;
                break;
                
            case STATE_PROCESSING:
                if (input_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                process_pos = 0;
                for (size_t i = 0; i < input_pos; i++) {
                    if (process_pos >= MAX_BUFFER_SIZE) {
                        break;
                    }
                    process_buffer[process_pos++] = TO_UPPER(input_buffer[i]);
                }
                process_buffer[process_pos] = '\0';
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                if (process_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                printf("Processed result: %s\n", process_buffer);
                printf("Original length: %zu\n", input_pos);
                printf("Processed length: %zu\n", process_pos);
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
    
    return current_state == STATE_WRITING ? EXIT_SUCCESS : EXIT_FAILURE;
}
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
#define VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    size_t input_pos = 0;
    size_t process_pos = 0;
    int input_char;
    
    printf("Enter text (max %d chars, Ctrl+D to end):\n", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                input_char = getchar();
                if (input_char == EOF) {
                    if (input_pos > 0) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        printf("No input received.\n");
                        return 0;
                    }
                } else if (input_char == '\n') {
                    if (input_pos > 0) {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else {
                    if (input_pos < MAX_INPUT_LEN && VALID_CHAR(input_char)) {
                        input_buffer[input_pos++] = (char)input_char;
                        TRANSITION(STATE_READING);
                    } else {
                        printf("Invalid input or buffer full.\n");
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_READING:
                input_char = getchar();
                if (input_char == EOF || input_char == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    if (input_pos < MAX_INPUT_LEN && VALID_CHAR(input_char)) {
                        input_buffer[input_pos++] = (char)input_char;
                    } else {
                        printf("Invalid input or buffer full.\n");
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                process_pos = 0;
                for (size_t i = 0; i < input_pos && process_pos < MAX_BUFFER_SIZE; i++) {
                    char c = input_buffer[i];
                    if (c >= 'a' && c <= 'z') {
                        process_buffer[process_pos++] = c - 32;
                    } else if (c >= 'A' && c <= 'Z') {
                        process_buffer[process_pos++] = c + 32;
                    } else if (VALID_CHAR(c)) {
                        process_buffer[process_pos++] = c;
                    }
                }
                
                if (process_pos > 0) {
                    process_buffer[process_pos] = '\0';
                    TRANSITION(STATE_WRITING);
                } else {
                    printf("No valid characters to process.\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed output: %s\n", process_buffer);
                printf("Character count: %zu\n", process_pos);
                
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(process_buffer, 0, sizeof(process_buffer));
                input_pos = 0;
                process_pos = 0;
                
                printf("Enter more text or Ctrl+D to exit:\n");
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("State machine encountered an error.\n");
        return 1;
    }
    
    return 0;
}
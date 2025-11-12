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
#define CHECK_BOUNDS(len) ((len) >= 0 && (len) < MAX_INPUT_LEN)

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (!CHECK_BOUNDS(len)) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void process_buffer(char* buffer, size_t len) {
    if (buffer == NULL || len == 0) return;
    for (size_t i = 0; i < len; i++) {
        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
            buffer[i] = buffer[i] - 'a' + 'A';
        }
    }
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_BUFFER_SIZE];
    size_t input_len = 0;
    size_t output_len = 0;
    int cycle_count = 0;
    
    printf("Enter text (max %d chars, empty line to exit):\n", MAX_INPUT_LEN-1);
    
    while (cycle_count < 100) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, MAX_INPUT_LEN, stdin) != NULL) {
                    input_len = strnlen(input_buffer, MAX_INPUT_LEN);
                    if (input_len > 0 && input_buffer[input_len-1] == '\n') {
                        input_buffer[input_len-1] = '\0';
                        input_len--;
                    }
                    if (input_len == 0) {
                        TRANSITION(STATE_ERROR);
                    } else {
                        TRANSITION(STATE_READING);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (validate_input(input_buffer, input_len)) {
                    output_len = (input_len < MAX_BUFFER_SIZE) ? input_len : MAX_BUFFER_SIZE-1;
                    strncpy(output_buffer, input_buffer, output_len);
                    output_buffer[output_len] = '\0';
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                process_buffer(output_buffer, output_len);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed: %s\n", output_buffer);
                printf("Enter next text (empty to exit):\n");
                TRANSITION(STATE_IDLE);
                cycle_count++;
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or system error\n");
                printf("Enter text again (empty to exit):\n");
                TRANSITION(STATE_IDLE);
                cycle_count++;
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
        
        if (input_len == 0 && current_state == STATE_IDLE) {
            break;
        }
    }
    
    printf("State machine terminated after %d cycles\n", cycle_count);
    return 0;
}
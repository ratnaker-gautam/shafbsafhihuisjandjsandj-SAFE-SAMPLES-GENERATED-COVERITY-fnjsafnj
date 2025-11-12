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
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN] = {0};
    char process_buffer[MAX_BUFFER_SIZE] = {0};
    int input_len = 0;
    int buffer_pos = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, MAX_INPUT_LEN, stdin) != NULL) {
                    input_len = (int)strlen(input_buffer);
                    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                        input_buffer[input_len - 1] = '\0';
                        input_len--;
                    }
                    if (CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                buffer_pos = 0;
                for (int i = 0; i < input_len && buffer_pos < MAX_BUFFER_SIZE - 1; i++) {
                    if (input_buffer[i] >= 'a' && input_buffer[i] <= 'z') {
                        process_buffer[buffer_pos++] = input_buffer[i] - 32;
                    } else if (input_buffer[i] >= 'A' && input_buffer[i] <= 'Z') {
                        process_buffer[buffer_pos++] = input_buffer[i];
                    }
                }
                process_buffer[buffer_pos] = '\0';
                TRANSITION(STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING:
                if (buffer_pos > 0) {
                    for (int i = 0; i < buffer_pos / 2; i++) {
                        char temp = process_buffer[i];
                        process_buffer[i] = process_buffer[buffer_pos - 1 - i];
                        process_buffer[buffer_pos - 1 - i] = temp;
                    }
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed result: %s\n", process_buffer);
                printf("Enter new text (max %d chars) or Ctrl+D to exit: ", MAX_INPUT_LEN - 1);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid state transition\n");
                return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}
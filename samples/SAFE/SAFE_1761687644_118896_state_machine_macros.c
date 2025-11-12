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
    int processed_count = 0;
    
    printf("Enter text (max %d chars, empty line to finish):\n", MAX_INPUT_LEN-1);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, MAX_INPUT_LEN, stdin) != NULL) {
                    input_len = (int)strlen(input_buffer);
                    if (input_len > 0 && input_buffer[input_len-1] == '\n') {
                        input_buffer[input_len-1] = '\0';
                        input_len--;
                    }
                    
                    if (input_len == 0) {
                        printf("Processing complete. Processed %d items.\n", processed_count);
                        return 0;
                    }
                    
                    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    buffer_pos = 0;
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (buffer_pos < input_len) {
                    if (buffer_pos < MAX_BUFFER_SIZE-1) {
                        process_buffer[buffer_pos] = input_buffer[buffer_pos];
                        buffer_pos++;
                    } else {
                        TRANSITION(STATE_PROCESSING);
                    }
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (buffer_pos > 0 && buffer_pos < MAX_BUFFER_SIZE) {
                    process_buffer[buffer_pos] = '\0';
                    
                    for (int i = 0; i < buffer_pos; i++) {
                        if (process_buffer[i] >= 'a' && process_buffer[i] <= 'z') {
                            process_buffer[i] = process_buffer[i] - 'a' + 'A';
                        }
                    }
                    
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed: %s\n", process_buffer);
                processed_count++;
                
                if (processed_count < 1000) {
                    TRANSITION(STATE_IDLE);
                } else {
                    printf("Maximum processing limit reached.\n");
                    return 0;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid state transition or input.\n");
                return 1;
        }
    }
    
    return 1;
}
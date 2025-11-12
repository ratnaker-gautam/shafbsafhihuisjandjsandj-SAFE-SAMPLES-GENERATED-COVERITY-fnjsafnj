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
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    int input_len = 0;
    int process_len = 0;
    char ch;
    
    printf("Enter text (max %d chars, 'q' to quit): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (scanf("%c", &ch) == 1) {
                    if (ch == 'q') {
                        printf("Quitting...\n");
                        return 0;
                    }
                    if (CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
                        input_buffer[input_len++] = ch;
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_READING:
                if (scanf("%c", &ch) == 1) {
                    if (ch == '\n') {
                        input_buffer[input_len] = '\0';
                        TRANSITION(STATE_PROCESSING);
                    } else if (ch == 'q') {
                        printf("Quitting...\n");
                        return 0;
                    } else {
                        if (CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
                            input_buffer[input_len++] = ch;
                        } else {
                            TRANSITION(STATE_ERROR);
                        }
                    }
                }
                break;
                
            case STATE_PROCESSING:
                process_len = 0;
                for (int i = 0; i < input_len && CHECK_BOUNDS(process_len, MAX_BUFFER_SIZE); i++) {
                    if (input_buffer[i] >= 'a' && input_buffer[i] <= 'z') {
                        process_buffer[process_len++] = input_buffer[i] - 32;
                    } else if (input_buffer[i] >= 'A' && input_buffer[i] <= 'Z') {
                        process_buffer[process_len++] = input_buffer[i] + 32;
                    } else if (input_buffer[i] >= '0' && input_buffer[i] <= '9') {
                        process_buffer[process_len++] = input_buffer[i];
                    }
                }
                if (CHECK_BOUNDS(process_len, MAX_BUFFER_SIZE)) {
                    process_buffer[process_len] = '\0';
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed: %s\n", process_buffer);
                input_len = 0;
                process_len = 0;
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(process_buffer, 0, sizeof(process_buffer));
                printf("Enter text (max %d chars, 'q' to quit): ", MAX_INPUT_LEN);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error: Buffer overflow detected\n");
                return 1;
        }
    }
    
    return 0;
}
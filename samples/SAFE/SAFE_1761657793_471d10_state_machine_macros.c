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
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN] = {0};
    char output_buffer[MAX_OUTPUT_LEN] = {0};
    int input_len = 0;
    int output_len = 0;
    int cycle_count = 0;
    char ch;
    
    printf("State Machine Demo - Enter text (max %d chars, 'q' to quit):\n", MAX_INPUT_LEN-1);
    
    while (cycle_count < 1000) {
        switch (current_state) {
            case STATE_IDLE:
                if (scanf(" %c", &ch) == 1) {
                    if (ch == 'q') {
                        TRANSITION(STATE_WRITING);
                    } else {
                        ungetc(ch, stdin);
                        TRANSITION(STATE_READING);
                    }
                }
                break;
                
            case STATE_READING:
                if (fgets(input_buffer, MAX_INPUT_LEN, stdin) != NULL) {
                    input_len = strnlen(input_buffer, MAX_INPUT_LEN);
                    if (CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
                        if (input_len > 0 && input_buffer[input_len-1] == '\n') {
                            input_buffer[input_len-1] = '\0';
                            input_len--;
                        }
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                output_len = 0;
                for (int i = 0; i < input_len && CHECK_BOUNDS(output_len, MAX_OUTPUT_LEN-1); i++) {
                    char c = input_buffer[i];
                    if (c >= 'a' && c <= 'z') {
                        output_buffer[output_len++] = c - 'a' + 'A';
                    } else if (c >= 'A' && c <= 'Z') {
                        output_buffer[output_len++] = c - 'A' + 'a';
                    } else if (c >= '0' && c <= '9') {
                        output_buffer[output_len++] = c;
                    } else {
                        output_buffer[output_len++] = '_';
                    }
                }
                if (CHECK_BOUNDS(output_len, MAX_OUTPUT_LEN)) {
                    output_buffer[output_len] = '\0';
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                if (output_len > 0) {
                    printf("Processed: %s\n", output_buffer);
                }
                printf("Enter more text or 'q' to quit: ");
                fflush(stdout);
                input_len = 0;
                output_len = 0;
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(output_buffer, 0, sizeof(output_buffer));
                TRANSITION(STATE_IDLE);
                if (ch == 'q') {
                    printf("Exiting state machine.\n");
                    return 0;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid state transition or buffer overflow\n");
                return 1;
                
            default:
                printf("Error: Unknown state\n");
                return 1;
        }
        
        cycle_count++;
        if (cycle_count >= 1000) {
            printf("Cycle limit reached\n");
            break;
        }
    }
    
    return 0;
}
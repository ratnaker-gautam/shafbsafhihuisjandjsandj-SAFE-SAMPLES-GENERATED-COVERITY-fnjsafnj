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
#define STATE_DONE 5

#define MAX_INPUT_LEN 100
#define MAX_BUFFER_SIZE 50

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER_SIZE + 1] = {0};
    int input_len = 0;
    int process_len = 0;
    int valid_count = 0;
    int invalid_count = 0;
    
    printf("Enter numbers separated by spaces (max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
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
                process_len = 0;
                valid_count = 0;
                invalid_count = 0;
                TRANSITION(STATE_PROCESSING);
                break;
                
            case STATE_PROCESSING: {
                int i = 0;
                int num_start = -1;
                
                while (i <= input_len) {
                    char c = input_buffer[i];
                    
                    if (IS_DIGIT(c)) {
                        if (num_start == -1) {
                            num_start = i;
                        }
                    } else if (c == ' ' || c == '\0') {
                        if (num_start != -1) {
                            int num_len = i - num_start;
                            if (CHECK_BOUNDS(num_len, MAX_BUFFER_SIZE)) {
                                strncpy(process_buffer, &input_buffer[num_start], num_len);
                                process_buffer[num_len] = '\0';
                                
                                char *endptr;
                                long value = strtol(process_buffer, &endptr, 10);
                                if (endptr != process_buffer && *endptr == '\0' && 
                                    value >= 0 && value <= 1000) {
                                    valid_count++;
                                } else {
                                    invalid_count++;
                                }
                            } else {
                                invalid_count++;
                            }
                            num_start = -1;
                        }
                    } else {
                        if (num_start != -1) {
                            invalid_count++;
                            num_start = -1;
                        }
                    }
                    i++;
                }
                TRANSITION(STATE_WRITING);
                break;
            }
                
            case STATE_WRITING:
                printf("Valid numbers: %d\n", valid_count);
                printf("Invalid entries: %d\n", invalid_count);
                if (valid_count > 0) {
                    printf("Processing completed successfully.\n");
                    TRANSITION(STATE_DONE);
                } else {
                    printf("No valid numbers found.\n");
                    TRANSITION(STATE_DONE);
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow.\n");
                TRANSITION(STATE_DONE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return current_state == STATE_DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}
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
#define MAX_BUFFER 50

#define TRANSITION_OK 0
#define TRANSITION_ERROR -1

#define CHECK_BOUNDS(val, max) ((val) >= 0 && (val) < (max))

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

int state_transition(int current_state, int event, char* buffer, size_t* buf_len) {
    if (!CHECK_BOUNDS(current_state, 5) || !CHECK_BOUNDS(event, 3)) {
        return TRANSITION_ERROR;
    }
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == 0) {
                if (buffer != NULL && buf_len != NULL) {
                    *buf_len = 0;
                }
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == 1) {
                return STATE_PROCESSING;
            } else if (event == 2) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == 1) {
                if (buffer != NULL && buf_len != NULL && *buf_len > 0) {
                    for (size_t i = 0; i < *buf_len; i++) {
                        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                            buffer[i] = buffer[i] - 'a' + 'A';
                        }
                    }
                }
                return STATE_WRITING;
            } else if (event == 2) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (event == 0) {
                return STATE_IDLE;
            } else if (event == 2) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            if (event == 0) {
                return STATE_IDLE;
            }
            break;
    }
    
    return current_state;
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN];
    char process_buffer[MAX_BUFFER];
    size_t process_len = 0;
    int running = 1;
    
    printf("State Machine Demo - Enter commands:\n");
    printf("0: Start reading, 1: Process, 2: Error, 3: Quit\n");
    
    while (running) {
        printf("Current state: %d > ", current_state);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        
        if (!validate_input(input_buffer, input_len)) {
            printf("Invalid input\n");
            continue;
        }
        
        int command;
        if (sscanf(input_buffer, "%d", &command) != 1) {
            printf("Invalid command\n");
            continue;
        }
        
        if (command == 3) {
            running = 0;
            continue;
        }
        
        if (current_state == STATE_READING && command == 1) {
            if (input_len > 1) {
                size_t copy_len = input_len - 2;
                if (copy_len > MAX_BUFFER - 1) {
                    copy_len = MAX_BUFFER - 1;
                }
                strncpy(process_buffer, input_buffer + 2, copy_len);
                process_buffer[copy_len] = '\0';
                process_len = copy_len;
            }
        }
        
        int new_state = state_transition(current_state, command, process_buffer, &process_len);
        
        if (new_state == TRANSITION_ERROR) {
            printf("Transition error\n");
            current_state = STATE_ERROR;
        } else {
            current_state = new_state;
        }
        
        if (current_state == STATE_WRITING && process_len > 0) {
            printf("Processed data: %.*s\n", (int)process_len, process_buffer);
        }
        
        if (current_state == STATE_ERROR) {
            printf("Error state reached\n");
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}
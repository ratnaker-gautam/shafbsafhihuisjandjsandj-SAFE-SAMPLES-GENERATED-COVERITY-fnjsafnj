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

int transition_state(int current_state, int event, char* buffer, size_t* buf_len) {
    if (!CHECK_BOUNDS(current_state, 5) || !CHECK_BOUNDS(event, 3)) {
        return STATE_ERROR;
    }
    
    switch (current_state) {
        case STATE_IDLE:
            if (event == 1) {
                if (buffer != NULL && buf_len != NULL) {
                    *buf_len = 0;
                }
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == 0) {
                return STATE_IDLE;
            } else if (event == 1) {
                return STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == 0) {
                return STATE_READING;
            } else if (event == 1) {
                if (buffer != NULL && buf_len != NULL && *buf_len > 0) {
                    for (size_t i = 0; i < *buf_len; i++) {
                        if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                            buffer[i] = buffer[i] - 'a' + 'A';
                        }
                    }
                }
                return STATE_WRITING;
            }
            break;
            
        case STATE_WRITING:
            if (event == 0) {
                return STATE_IDLE;
            } else if (event == 1) {
                return STATE_READING;
            }
            break;
            
        case STATE_ERROR:
            if (event == 0) {
                return STATE_IDLE;
            }
            break;
    }
    
    return STATE_ERROR;
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER + 1] = {0};
    size_t buf_len = 0;
    int running = 1;
    
    printf("State Machine Demo - Enter commands (0=back, 1=next, 2=quit):\n");
    
    while (running) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n> ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            current_state = STATE_ERROR;
            continue;
        }
        
        size_t input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[--input_len] = '\0';
        }
        
        if (!validate_input(input_buffer, input_len)) {
            current_state = STATE_ERROR;
            continue;
        }
        
        int event;
        if (strcmp(input_buffer, "0") == 0) {
            event = 0;
        } else if (strcmp(input_buffer, "1") == 0) {
            event = 1;
        } else if (strcmp(input_buffer, "2") == 0) {
            event = 2;
        } else {
            current_state = STATE_ERROR;
            continue;
        }
        
        if (event == 2) {
            running = 0;
            continue;
        }
        
        if (current_state == STATE_READING && event == 1) {
            printf("Enter text to process (max %d chars): ", MAX_BUFFER);
            if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                current_state = STATE_ERROR;
                continue;
            }
            
            input_len = strlen(input_buffer);
            if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                input_buffer[--input_len] = '\0';
            }
            
            if (!validate_input(input_buffer, input_len) || input_len > MAX_BUFFER) {
                current_state = STATE_ERROR;
                continue;
            }
            
            strncpy(process_buffer, input_buffer, MAX_BUFFER);
            process_buffer[MAX_BUFFER] = '\0';
            buf_len = input_len;
        }
        
        if (current_state == STATE_WRITING && event == 1) {
            printf("Processed output: %s\n", process_buffer);
        }
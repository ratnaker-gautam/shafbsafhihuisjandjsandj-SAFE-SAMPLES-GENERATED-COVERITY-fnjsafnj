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

#define TRANSITION(new_state) do { current_state = new_state; } while(0)
#define CHECK_BOUNDS(len, max) ((len) >= 0 && (len) < (max))

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char process_buffer[MAX_BUFFER + 1] = {0};
    int input_len = 0;
    int process_len = 0;
    char user_input[MAX_INPUT_LEN + 1];
    
    printf("State Machine Demo - Enter commands (type 'quit' to exit):\n");
    
    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                printf("IDLE> ");
                if (fgets(user_input, sizeof(user_input), stdin) != NULL) {
                    size_t len = strlen(user_input);
                    if (len > 0 && user_input[len - 1] == '\n') {
                        user_input[len - 1] = '\0';
                        len--;
                    }
                    
                    if (!CHECK_BOUNDS(len, MAX_INPUT_LEN)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (strcmp(user_input, "quit") == 0) {
                        printf("Exiting state machine.\n");
                        return 0;
                    } else if (strcmp(user_input, "read") == 0) {
                        TRANSITION(STATE_READING);
                    } else {
                        printf("Unknown command. Use 'read', 'quit'.\n");
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                printf("READING - Enter text to process: ");
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    input_len = strlen(input_buffer);
                    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                        input_buffer[input_len - 1] = '\0';
                        input_len--;
                    }
                    
                    if (!CHECK_BOUNDS(input_len, MAX_INPUT_LEN)) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (input_len > 0) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        printf("No input received.\n");
                        TRANSITION(STATE_IDLE);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                printf("PROCESSING input...\n");
                process_len = 0;
                
                for (int i = 0; i < input_len && process_len < MAX_BUFFER; i++) {
                    if (input_buffer[i] >= 'a' && input_buffer[i] <= 'z') {
                        process_buffer[process_len++] = input_buffer[i] - 32;
                    } else if (input_buffer[i] >= 'A' && input_buffer[i] <= 'Z') {
                        process_buffer[process_len++] = input_buffer[i] + 32;
                    } else if (input_buffer[i] == ' ') {
                        process_buffer[process_len++] = '_';
                    }
                }
                
                process_buffer[process_len] = '\0';
                printf("Processed: %s\n", process_buffer);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("WRITING result to console.\n");
                printf("Final output: %s\n", process_buffer);
                printf("Processing complete.\n");
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("ERROR: Invalid operation detected.\n");
                printf("Resetting to IDLE state.\n");
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(process_buffer, 0, sizeof(process_buffer));
                input_len = 0;
                process_len = 0;
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return 0;
}
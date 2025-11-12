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
#define IS_VALID_CHAR(c) ((c) >= 32 && (c) <= 126)

static int process_data(const char* input, char* output, size_t len) {
    if (input == NULL || output == NULL || len == 0) return -1;
    if (len > MAX_INPUT_LEN) return -1;
    
    size_t out_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return -1;
        if (out_idx >= MAX_OUTPUT_LEN - 2) return -1;
        
        output[out_idx++] = input[i];
        if (i % 5 == 4 && out_idx < MAX_OUTPUT_LEN - 2) {
            output[out_idx++] = '-';
        }
    }
    output[out_idx] = '\0';
    return 0;
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    size_t input_length = 0;
    int choice = 0;
    
    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                printf("State Machine Menu:\n");
                printf("1. Enter input data\n");
                printf("2. Exit\n");
                printf("Choice: ");
                
                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n');
                    printf("Invalid input\n");
                    break;
                }
                
                if (choice == 1) {
                    TRANSITION(STATE_READING);
                } else if (choice == 2) {
                    return 0;
                } else {
                    printf("Invalid choice\n");
                }
                break;
                
            case STATE_READING:
                printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
                while (getchar() != '\n');
                
                if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                input_length = strlen(input_buffer);
                if (input_length > 0 && input_buffer[input_length - 1] == '\n') {
                    input_buffer[--input_length] = '\0';
                }
                
                if (input_length == 0) {
                    printf("Empty input\n");
                    TRANSITION(STATE_IDLE);
                } else {
                    TRANSITION(STATE_PROCESSING);
                }
                break;
                
            case STATE_PROCESSING:
                if (process_data(input_buffer, output_buffer, input_length) == 0) {
                    TRANSITION(STATE_WRITING);
                } else {
                    printf("Data processing failed\n");
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                printf("Processed output: %s\n", output_buffer);
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(output_buffer, 0, sizeof(output_buffer));
                input_length = 0;
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error occurred. Resetting...\n");
                memset(input_buffer, 0, sizeof(input_buffer));
                memset(output_buffer, 0, sizeof(output_buffer));
                input_length = 0;
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }
    
    return 0;
}
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

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

static void process_data(const char* input, char* output) {
    size_t len = strlen(input);
    size_t out_idx = 0;
    
    for (size_t i = 0; i < len && out_idx < MAX_OUTPUT_LEN - 1; i++) {
        if (input[i] >= 'a' && input[i] <= 'z') {
            output[out_idx++] = input[i] - 32;
        } else if (input[i] >= 'A' && input[i] <= 'Z') {
            output[out_idx++] = input[i] + 32;
        } else if (input[i] >= '0' && input[i] <= '9') {
            if (out_idx < MAX_OUTPUT_LEN - 3) {
                output[out_idx++] = '[';
                output[out_idx++] = input[i];
                output[out_idx++] = ']';
            }
        } else {
            output[out_idx++] = input[i];
        }
    }
    output[out_idx] = '\0';
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    char output_buffer[MAX_OUTPUT_LEN + 1];
    int result;
    
    while (1) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (or 'quit' to exit): ");
                TRANSITION(STATE_READING);
                break;
                
            case STATE_READING:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    size_t len = strlen(input_buffer);
                    if (len > 0 && input_buffer[len - 1] == '\n') {
                        input_buffer[len - 1] = '\0';
                    }
                    if (strcmp(input_buffer, "quit") == 0) {
                        printf("Goodbye!\n");
                        return 0;
                    }
                    if (validate_input(input_buffer)) {
                        TRANSITION(STATE_PROCESSING);
                    } else {
                        printf("Invalid input. Please enter printable ASCII characters only.\n");
                        TRANSITION(STATE_IDLE);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                memset(output_buffer, 0, sizeof(output_buffer));
                process_data(input_buffer, output_buffer);
                TRANSITION(STATE_WRITING);
                break;
                
            case STATE_WRITING:
                printf("Processed: %s\n", output_buffer);
                TRANSITION(STATE_IDLE);
                break;
                
            case STATE_ERROR:
                printf("Error occurred during input reading.\n");
                TRANSITION(STATE_IDLE);
                break;
                
            default:
                current_state = STATE_IDLE;
                break;
        }
    }
    
    return 0;
}
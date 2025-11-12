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

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!IS_VALID_CHAR(input[i])) return 0;
    }
    return 1;
}

static void process_data(const char *input, char *output) {
    size_t len = strlen(input);
    size_t j = 0;
    for (size_t i = 0; i < len && j < MAX_OUTPUT_LEN - 1; i++) {
        if (input[i] != ' ') {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    int result = 0;

    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    size_t len = strlen(input_buffer);
                    if (len > 0 && input_buffer[len - 1] == '\n') {
                        input_buffer[len - 1] = '\0';
                    }
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;

            case STATE_READING:
                if (validate_input(input_buffer)) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    printf("Invalid input\n");
                    TRANSITION(STATE_IDLE);
                }
                break;

            case STATE_PROCESSING:
                process_data(input_buffer, output_buffer);
                TRANSITION(STATE_WRITING);
                break;

            case STATE_WRITING:
                printf("Processed: %s\n", output_buffer);
                printf("Continue? (y/n): ");
                char response[16];
                if (fgets(response, sizeof(response), stdin) != NULL) {
                    if (response[0] == 'y' || response[0] == 'Y') {
                        TRANSITION(STATE_IDLE);
                    } else {
                        result = 0;
                        TRANSITION(STATE_ERROR);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;

            default:
                TRANSITION(STATE_ERROR);
                break;
        }
    }

    return result;
}
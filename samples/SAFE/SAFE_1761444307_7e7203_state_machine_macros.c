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

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_OUTPUT_LEN + 1] = {0};
    size_t input_pos = 0;
    size_t output_pos = 0;
    int ch;
    
    printf("Enter text (max %d chars, Ctrl+D to end):\n", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                ch = getchar();
                if (ch == EOF) {
                    TRANSITION(STATE_WRITING);
                } else if (ch == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else if (IS_VALID_CHAR(ch) && input_pos < MAX_INPUT_LEN) {
                    input_buffer[input_pos++] = (char)ch;
                    TRANSITION(STATE_READING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                ch = getchar();
                if (ch == EOF) {
                    TRANSITION(STATE_PROCESSING);
                } else if (ch == '\n') {
                    TRANSITION(STATE_PROCESSING);
                } else if (IS_VALID_CHAR(ch) && input_pos < MAX_INPUT_LEN) {
                    input_buffer[input_pos++] = (char)ch;
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos == 0) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                
                for (size_t i = 0; i < input_pos; i++) {
                    char c = input_buffer[i];
                    if (output_pos + 3 >= MAX_OUTPUT_LEN) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    
                    if (c >= 'a' && c <= 'z') {
                        output_buffer[output_pos++] = c - 32;
                    } else if (c >= 'A' && c <= 'Z') {
                        output_buffer[output_pos++] = c + 32;
                    } else if (IS_VALID_CHAR(c)) {
                        output_buffer[output_pos++] = c;
                    } else {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                }
                
                if (current_state != STATE_ERROR) {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_WRITING:
                if (output_pos > 0) {
                    output_buffer[output_pos] = '\0';
                    printf("Processed output: %s\n", output_buffer);
                } else {
                    printf("No valid input processed.\n");
                }
                TRANSITION(STATE_ERROR);
                break;
                
            case STATE_ERROR:
                if (input_pos >= MAX_INPUT_LEN) {
                    printf("Error: Input too long.\n");
                } else if (output_pos >= MAX_OUTPUT_LEN) {
                    printf("Error: Output buffer overflow.\n");
                } else {
                    printf("Error: Invalid input character or state.\n");
                }
                return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}
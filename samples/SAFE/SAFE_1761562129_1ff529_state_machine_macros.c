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
#define IS_VALID_CHAR(c) ((c) >= 'a' && (c) <= 'z')
#define TO_UPPER(c) ((c) - 32)

int main(void) {
    int current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_BUFFER_SIZE + 1] = {0};
    size_t input_len = 0;
    size_t read_pos = 0;
    size_t write_pos = 0;
    int c;
    
    printf("Enter text (a-z only, max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_IDLE:
                if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                    input_len = strlen(input_buffer);
                    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
                        input_buffer[--input_len] = '\0';
                    }
                    if (input_len > MAX_INPUT_LEN) {
                        TRANSITION(STATE_ERROR);
                        break;
                    }
                    for (size_t i = 0; i < input_len; i++) {
                        if (!IS_VALID_CHAR(input_buffer[i]) && input_buffer[i] != ' ') {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                    }
                    if (current_state != STATE_ERROR) {
                        TRANSITION(STATE_READING);
                    }
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_READING:
                if (read_pos < input_len) {
                    TRANSITION(STATE_PROCESSING);
                } else {
                    TRANSITION(STATE_WRITING);
                }
                break;
                
            case STATE_PROCESSING:
                if (write_pos >= MAX_BUFFER_SIZE) {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                c = input_buffer[read_pos++];
                if (c == ' ') {
                    output_buffer[write_pos++] = c;
                } else if (IS_VALID_CHAR(c)) {
                    output_buffer[write_pos++] = TO_UPPER(c);
                } else {
                    TRANSITION(STATE_ERROR);
                    break;
                }
                TRANSITION(STATE_READING);
                break;
                
            case STATE_WRITING:
                if (write_pos > 0) {
                    output_buffer[write_pos] = '\0';
                    printf("Processed text: %s\n", output_buffer);
                    TRANSITION(STATE_IDLE);
                    read_pos = 0;
                    write_pos = 0;
                    memset(input_buffer, 0, sizeof(input_buffer));
                    memset(output_buffer, 0, sizeof(output_buffer));
                    printf("Enter text (a-z only, max %d chars): ", MAX_INPUT_LEN);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return EXIT_FAILURE;
        }
    }
    
    return EXIT_SUCCESS;
}
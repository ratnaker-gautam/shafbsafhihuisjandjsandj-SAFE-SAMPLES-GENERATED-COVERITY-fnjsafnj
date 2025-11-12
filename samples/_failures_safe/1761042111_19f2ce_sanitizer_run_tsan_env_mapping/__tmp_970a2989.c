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
                } else if (IS_VALID_CHAR(ch)) {
                    if (input_pos < MAX_INPUT_LEN) {
                        input_buffer[input_pos++] = (char)ch;
                        TRANSITION(STATE_READING);
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_READING:
                ch = getchar();
                if (ch == EOF) {
                    TRANSITION(STATE_PROCESSING);
                } else if (IS_VALID_CHAR(ch)) {
                    if (input_pos < MAX_INPUT_LEN) {
                        input_buffer[input_pos++] = (char)ch;
                    } else {
                        TRANSITION(STATE_ERROR);
                    }
                }
                break;
                
            case STATE_PROCESSING:
                if (input_pos > 0) {
                    for (size_t i = 0; i < input_pos; i++) {
                        if (output_pos < MAX_OUTPUT_LEN - 2) {
                            output_buffer[output_pos++] = input_buffer[i];
                            if (i % 10 == 9 && i < input_pos - 1) {
                                output_buffer[output_pos++] = '\n';
                            }
                        } else {
                            TRANSITION(STATE_ERROR);
                            break;
                        }
                    }
                    output_buffer[output_pos] = '\0';
                    TRANSITION(STATE_WRITING);
                } else {
                    TRANSITION(STATE_ERROR);
                }
                break;
                
            case STATE_WRITING:
                if (output_pos > 0) {
                    printf("Processed output (%zu chars):\n", output_pos);
                    if (fwrite(output_buffer, 1, output_pos, stdout) != output_pos) {
                        TRANSITION(STATE_ERROR);
                    } else {
                        printf("\nProcessing complete.\n");
                        return EXIT_SUCCESS;
                    }
                } else {
                    printf("No input received.\n");
                    return EXIT_SUCCESS;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Processing failed.\n");
                return EXIT_FAILURE;
        }
    }
    
    return EXIT_FAILURE;
}
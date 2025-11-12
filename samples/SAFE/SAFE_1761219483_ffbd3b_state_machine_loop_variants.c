//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    enum state current_state = STATE_START;
    char buffer[256];
    size_t buffer_pos = 0;
    int input_char;
    int word_count = 0;
    int valid_input = 1;
    
    while (current_state != STATE_END) {
        switch (current_state) {
            case STATE_START:
                printf("Enter text (max 255 chars, Ctrl+D to end): ");
                buffer_pos = 0;
                memset(buffer, 0, sizeof(buffer));
                current_state = STATE_READING;
                break;
                
            case STATE_READING:
                input_char = getchar();
                if (input_char == EOF) {
                    if (buffer_pos > 0) {
                        current_state = STATE_PROCESSING;
                    } else {
                        current_state = STATE_END;
                    }
                } else if (input_char == '\n') {
                    current_state = STATE_PROCESSING;
                } else if (buffer_pos < sizeof(buffer) - 1) {
                    if (isprint(input_char)) {
                        buffer[buffer_pos++] = (char)input_char;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                if (buffer_pos > 0) {
                    int in_word = 0;
                    for (size_t i = 0; i < buffer_pos; i++) {
                        if (isalpha(buffer[i])) {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else if (isspace(buffer[i])) {
                            in_word = 0;
                        }
                    }
                    current_state = STATE_OUTPUT;
                } else {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_OUTPUT:
                printf("Word count: %d\n", word_count);
                word_count = 0;
                current_state = STATE_START;
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input detected\n");
                valid_input = 0;
                current_state = STATE_END;
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (valid_input) {
        printf("Processing completed successfully\n");
    } else {
        printf("Processing terminated due to error\n");
    }
    
    return valid_input ? EXIT_SUCCESS : EXIT_FAILURE;
}
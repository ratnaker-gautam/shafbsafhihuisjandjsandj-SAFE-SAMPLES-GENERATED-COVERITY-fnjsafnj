//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    size_t buffer_pos = 0;
    int input_char;
    int word_count = 0;
    int line_count = 0;
    
    while (current_state != DONE && current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (press Ctrl+D to finish):\n");
                current_state = READING;
                break;
                
            case READING:
                input_char = getchar();
                if (input_char == EOF) {
                    if (buffer_pos > 0) {
                        current_state = PROCESSING;
                    } else {
                        current_state = DONE;
                    }
                } else if (input_char == '\n') {
                    line_count++;
                    if (buffer_pos > 0) {
                        current_state = PROCESSING;
                    }
                } else if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)input_char;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    buffer[buffer_pos] = '\0';
                    int in_word = 0;
                    for (size_t i = 0; i < buffer_pos; i++) {
                        if (isalpha((unsigned char)buffer[i])) {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                    }
                    current_state = WRITING;
                } else {
                    current_state = READING;
                }
                break;
                
            case WRITING:
                if (buffer_pos > 0) {
                    printf("Processed: %s\n", buffer);
                    buffer_pos = 0;
                }
                current_state = READING;
                break;
                
            case ERROR:
                printf("Error: Buffer overflow\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Summary: %d words, %d lines\n", word_count, line_count);
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
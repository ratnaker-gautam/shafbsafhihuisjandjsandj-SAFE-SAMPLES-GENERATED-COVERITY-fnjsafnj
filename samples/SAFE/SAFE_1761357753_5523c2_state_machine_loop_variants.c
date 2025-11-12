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
                printf("Enter text (empty line to finish):\n");
                current_state = READING;
                break;
                
            case READING:
                input_char = getchar();
                if (input_char == EOF) {
                    current_state = DONE;
                } else if (input_char == '\n') {
                    if (buffer_pos == 0) {
                        current_state = DONE;
                    } else {
                        buffer[buffer_pos] = '\0';
                        current_state = PROCESSING;
                    }
                } else if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)input_char;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                {
                    int in_word = 0;
                    size_t i = 0;
                    while (i < buffer_pos) {
                        if (isalpha((unsigned char)buffer[i])) {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                        i++;
                    }
                    line_count++;
                    current_state = WRITING;
                }
                break;
                
            case WRITING:
                printf("Line %d: '%s' (words: %d)\n", line_count, buffer, word_count);
                buffer_pos = 0;
                word_count = 0;
                current_state = READING;
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Buffer overflow\n");
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("Processing complete. Total lines: %d\n", line_count);
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
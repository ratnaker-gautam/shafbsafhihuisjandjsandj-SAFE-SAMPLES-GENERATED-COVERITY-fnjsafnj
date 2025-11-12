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
                } else if (isprint(input_char)) {
                    if (buffer_pos < sizeof(buffer) - 1) {
                        buffer[buffer_pos++] = (char)input_char;
                    } else {
                        current_state = ERROR;
                    }
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    current_state = WRITING;
                } else {
                    current_state = READING;
                }
                break;
                
            case WRITING:
                printf("Word %d: %s\n", word_count, buffer);
                buffer_pos = 0;
                current_state = READING;
                break;
                
            case ERROR:
                fprintf(stderr, "Error: Buffer overflow or invalid input\n");
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("\nSummary: %d words, %d lines\n", word_count, line_count);
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
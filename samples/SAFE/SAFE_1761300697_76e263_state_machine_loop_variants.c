//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, WRITING, ERROR };

int main(void) {
    enum state current_state = IDLE;
    char buffer[256];
    size_t buffer_pos = 0;
    int input_char;
    int word_count = 0;
    int line_count = 0;
    
    while (1) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (Ctrl+D to finish): ");
                current_state = READING;
                buffer_pos = 0;
                memset(buffer, 0, sizeof(buffer));
                break;
                
            case READING:
                input_char = getchar();
                if (input_char == EOF) {
                    if (buffer_pos > 0) {
                        current_state = PROCESSING;
                    } else {
                        current_state = WRITING;
                    }
                } else if (input_char == '\n') {
                    line_count++;
                    if (buffer_pos > 0) {
                        current_state = PROCESSING;
                    } else {
                        current_state = IDLE;
                    }
                } else if (buffer_pos < sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)input_char;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
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
                }
                current_state = IDLE;
                break;
                
            case WRITING:
                printf("\nResults:\n");
                printf("Words: %d\n", word_count);
                printf("Lines: %d\n", line_count);
                return 0;
                
            case ERROR:
                fprintf(stderr, "Error: Buffer overflow\n");
                return 1;
        }
    }
    
    return 0;
}
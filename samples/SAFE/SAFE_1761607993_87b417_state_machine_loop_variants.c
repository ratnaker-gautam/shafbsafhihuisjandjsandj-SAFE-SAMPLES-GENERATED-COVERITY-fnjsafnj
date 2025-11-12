//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, FINISHED, ERROR };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    size_t buffer_pos = 0;
    int input_char;
    int word_count = 0;
    int line_count = 0;
    
    while (current_state != FINISHED && current_state != ERROR) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to finish):\n");
                current_state = READING;
                buffer_pos = 0;
                break;
                
            case READING:
                input_char = getchar();
                if (input_char == EOF) {
                    current_state = FINISHED;
                } else if (input_char == '\n') {
                    if (buffer_pos == 0) {
                        current_state = FINISHED;
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
                    line_count++;
                    printf("Processed line %d: %d words\n", line_count, word_count);
                    word_count = 0;
                }
                current_state = IDLE;
                break;
                
            case FINISHED:
                printf("Total lines processed: %d\n", line_count);
                break;
                
            case ERROR:
                printf("Error: Buffer overflow\n");
                break;
        }
    }
    
    return current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
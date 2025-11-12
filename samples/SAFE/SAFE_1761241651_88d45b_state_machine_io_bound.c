//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int ch;
    
    while (1) {
        switch (current_state) {
            case IDLE:
                printf("Enter text (empty line to finish): ");
                buffer_pos = 0;
                current_state = READING;
                break;
                
            case READING:
                ch = getchar();
                if (ch == EOF) {
                    current_state = ERROR;
                } else if (ch == '\n') {
                    if (buffer_pos == 0) {
                        current_state = WRITING;
                    } else {
                        if (buffer_pos < 255) {
                            buffer[buffer_pos] = '\0';
                            current_state = PROCESSING;
                        } else {
                            current_state = ERROR;
                        }
                    }
                } else if (buffer_pos < 255) {
                    buffer[buffer_pos++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                {
                    int in_word = 0;
                    for (int i = 0; i < buffer_pos; i++) {
                        if (isalpha((unsigned char)buffer[i])) {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                    }
                    current_state = IDLE;
                }
                break;
                
            case WRITING:
                printf("Total words counted: %d\n", word_count);
                return 0;
                
            case ERROR:
                fprintf(stderr, "Error: Input too long or read error\n");
                return 1;
        }
    }
    
    return 0;
}
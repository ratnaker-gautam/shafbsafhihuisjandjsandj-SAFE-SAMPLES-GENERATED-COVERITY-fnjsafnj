//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, WRITING, ERROR };

int main(void) {
    enum state current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (1) {
        switch (current_state) {
            case IDLE:
                buffer_pos = 0;
                word_count = 0;
                memset(buffer, 0, sizeof(buffer));
                current_state = READING;
                break;
                
            case READING:
                ch = getchar();
                if (ch == EOF) {
                    current_state = WRITING;
                } else if (buffer_pos < (int)sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                    if (isspace(ch) || ch == '\n') {
                        current_state = PROCESSING;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    int in_word = 0;
                    for (int i = 0; i < buffer_pos; i++) {
                        if (!isspace(buffer[i]) && buffer[i] != '\n') {
                            if (!in_word) {
                                word_count++;
                                in_word = 1;
                            }
                        } else {
                            in_word = 0;
                        }
                    }
                }
                current_state = READING;
                break;
                
            case WRITING:
                printf("Word count: %d\n", word_count);
                return 0;
                
            case ERROR:
                fprintf(stderr, "Error: Buffer overflow\n");
                return 1;
        }
    }
    
    return 0;
}
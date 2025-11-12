//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
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
    int char_count = 0;
    int ch;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (1) {
        switch (current_state) {
            case IDLE:
                buffer_pos = 0;
                word_count = 0;
                char_count = 0;
                memset(buffer, 0, sizeof(buffer));
                current_state = READING;
                break;
                
            case READING:
                ch = getchar();
                if (ch == EOF) {
                    current_state = WRITING;
                } else if (ch == '\n') {
                    current_state = PROCESSING;
                } else if (buffer_pos < (int)sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                    char_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
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
                    current_state = WRITING;
                } else {
                    current_state = IDLE;
                }
                break;
                
            case WRITING:
                if (buffer_pos > 0) {
                    printf("Line: %.*s\n", buffer_pos, buffer);
                    printf("Characters: %d, Words: %d\n", char_count, word_count);
                }
                
                if (ch == EOF) {
                    printf("Processing complete.\n");
                    return 0;
                } else {
                    current_state = IDLE;
                }
                break;
                
            case ERROR:
                printf("Error: Buffer overflow detected.\n");
                return 1;
        }
    }
    
    return 0;
}
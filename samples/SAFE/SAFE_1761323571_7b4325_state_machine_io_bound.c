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
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (current_state != ERROR) {
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
                    if (buffer_pos > 0) {
                        current_state = PROCESSING;
                    } else {
                        printf("No input provided.\n");
                        return 0;
                    }
                } else if (ch == '\n') {
                    current_state = PROCESSING;
                } else if (buffer_pos < (int)sizeof(buffer) - 1) {
                    buffer[buffer_pos++] = (char)ch;
                } else {
                    printf("Input too long.\n");
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
                    current_state = ERROR;
                }
                break;
                
            case WRITING:
                printf("Line: %.*s\n", buffer_pos, buffer);
                printf("Words: %d\n", word_count);
                printf("---\n");
                current_state = IDLE;
                break;
                
            case ERROR:
                printf("Error occurred.\n");
                break;
        }
    }
    
    return 1;
}
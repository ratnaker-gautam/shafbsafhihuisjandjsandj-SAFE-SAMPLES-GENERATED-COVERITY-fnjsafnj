//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

int main(void) {
    enum state current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int char_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars, Ctrl+D to end):\n");
    
    while (current_state != DONE && current_state != ERROR) {
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
                    if (buffer_pos > 0) {
                        current_state = PROCESSING;
                    } else {
                        current_state = DONE;
                    }
                } else if (ch == '\n') {
                    current_state = PROCESSING;
                } else if (buffer_pos < 255) {
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
                    current_state = DONE;
                }
                break;
                
            case WRITING:
                printf("Text: %.*s\n", buffer_pos, buffer);
                printf("Characters: %d\n", char_count);
                printf("Words: %d\n", word_count);
                printf("---\n");
                current_state = IDLE;
                break;
                
            case ERROR:
                printf("Error: Input too long or invalid\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Processing complete.\n");
    return 0;
}
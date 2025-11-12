//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, ERROR, DONE };

int main(void) {
    enum State current_state = IDLE;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int char_count = 0;
    int c;
    
    printf("Enter text (max 255 chars, Ctrl+D to end):\n");
    
    while (current_state != DONE && current_state != ERROR) {
        c = getchar();
        
        switch (current_state) {
            case IDLE:
                if (c == EOF) {
                    current_state = DONE;
                } else if (isalpha(c)) {
                    buffer[buffer_pos++] = (char)c;
                    char_count++;
                    current_state = READING;
                } else if (!isspace(c) && c != EOF) {
                    current_state = ERROR;
                }
                break;
                
            case READING:
                if (c == EOF || buffer_pos >= 255) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        buffer_pos = 0;
                    }
                    if (c == EOF) {
                        current_state = DONE;
                    } else {
                        current_state = IDLE;
                    }
                } else if (isalpha(c)) {
                    buffer[buffer_pos++] = (char)c;
                    char_count++;
                } else if (isspace(c)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        buffer_pos = 0;
                    }
                    current_state = IDLE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (c == EOF) {
                    current_state = DONE;
                } else {
                    current_state = IDLE;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid character detected\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
        
        if (buffer_pos >= 255) {
            buffer[255] = '\0';
            word_count++;
            buffer_pos = 0;
            current_state = IDLE;
        }
    }
    
    if (current_state == DONE) {
        printf("\nResults:\n");
        printf("Words: %d\n", word_count);
        printf("Characters: %d\n", char_count);
    }
    
    return current_state == ERROR ? 1 : 0;
}
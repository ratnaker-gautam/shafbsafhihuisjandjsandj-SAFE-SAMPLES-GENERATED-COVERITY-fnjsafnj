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
        
        if (c == EOF) {
            switch (current_state) {
                case IDLE:
                    current_state = DONE;
                    break;
                case READING:
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        current_state = PROCESSING;
                    } else {
                        current_state = DONE;
                    }
                    break;
                case PROCESSING:
                    current_state = DONE;
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
            continue;
        }
        
        if (buffer_pos >= 255) {
            current_state = ERROR;
            continue;
        }
        
        switch (current_state) {
            case IDLE:
                if (!isspace(c)) {
                    buffer[buffer_pos++] = (char)c;
                    char_count++;
                    current_state = READING;
                }
                break;
                
            case READING:
                if (isspace(c)) {
                    buffer[buffer_pos] = '\0';
                    current_state = PROCESSING;
                } else {
                    buffer[buffer_pos++] = (char)c;
                    char_count++;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    word_count++;
                    buffer_pos = 0;
                }
                
                if (!isspace(c)) {
                    buffer[buffer_pos++] = (char)c;
                    char_count++;
                    current_state = READING;
                }
                break;
                
            case ERROR:
                printf("Error: Input too long or invalid state\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == PROCESSING && buffer_pos > 0) {
        word_count++;
    }
    
    if (current_state == ERROR) {
        printf("Error: Input processing failed\n");
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Characters: %d\n", char_count);
    
    return 0;
}
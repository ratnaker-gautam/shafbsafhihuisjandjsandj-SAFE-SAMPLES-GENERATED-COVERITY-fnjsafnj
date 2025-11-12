//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    int c;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (1) {
        c = getchar();
        
        if (c == EOF) {
            if (current_state == READING || current_state == PROCESSING) {
                current_state = WRITING;
            } else if (current_state == IDLE) {
                break;
            }
        }
        
        switch (current_state) {
            case IDLE:
                if (c != EOF && c != '\n' && !isspace(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                    }
                    current_state = READING;
                }
                break;
                
            case READING:
                if (c == EOF || c == '\n' || isspace(c)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        current_state = PROCESSING;
                    } else {
                        current_state = IDLE;
                    }
                } else {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                    }
                }
                break;
                
            case PROCESSING:
                word_count++;
                printf("Word %d: %s\n", word_count, buffer);
                buffer_pos = 0;
                
                if (c == EOF) {
                    current_state = WRITING;
                } else if (c == '\n' || isspace(c)) {
                    current_state = IDLE;
                } else {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                    }
                    current_state = READING;
                }
                break;
                
            case WRITING:
                printf("\nStatistics:\n");
                printf("Words: %d\n", word_count);
                printf("Characters: %d\n", char_count);
                return 0;
                
            case ERROR:
                fprintf(stderr, "Error state reached\n");
                return 1;
        }
        
        if (current_state == ERROR) {
            break;
        }
    }
    
    return 0;
}
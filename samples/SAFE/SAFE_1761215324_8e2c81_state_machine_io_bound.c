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
    
    while (1) {
        switch (current_state) {
            case IDLE:
                ch = getchar();
                if (ch == EOF) {
                    current_state = WRITING;
                } else if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                        current_state = READING;
                    } else {
                        current_state = ERROR;
                    }
                } else if (ch == '\n' || ch == ' ' || ch == '\t') {
                    current_state = IDLE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING:
                ch = getchar();
                if (ch == EOF) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        current_state = PROCESSING;
                    } else {
                        current_state = WRITING;
                    }
                } else if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                        current_state = READING;
                    } else {
                        current_state = ERROR;
                    }
                } else if (ch == '\n' || ch == ' ' || ch == '\t') {
                    buffer[buffer_pos] = '\0';
                    current_state = PROCESSING;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    word_count++;
                    printf("Word %d: %s\n", word_count, buffer);
                    buffer_pos = 0;
                    current_state = IDLE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case WRITING:
                printf("\nTotal words processed: %d\n", word_count);
                return 0;
                
            case ERROR:
                printf("Error: Invalid input or buffer overflow\n");
                return 1;
        }
    }
    
    return 0;
}
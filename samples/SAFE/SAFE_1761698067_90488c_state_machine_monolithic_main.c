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
    int c;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (1) {
        switch (current_state) {
            case IDLE:
                c = getchar();
                if (c == EOF) {
                    current_state = WRITING;
                } else if (isalpha(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                        current_state = READING;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    char_count++;
                } else if (c == '\n') {
                    char_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING:
                c = getchar();
                if (c == EOF) {
                    if (buffer_pos > 0) {
                        word_count++;
                        buffer_pos = 0;
                    }
                    current_state = WRITING;
                } else if (isalpha(c)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)c;
                        char_count++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(c)) {
                    if (buffer_pos > 0) {
                        word_count++;
                        buffer_pos = 0;
                    }
                    char_count++;
                    current_state = IDLE;
                } else if (c == '\n') {
                    if (buffer_pos > 0) {
                        word_count++;
                        buffer_pos = 0;
                    }
                    char_count++;
                    current_state = IDLE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                if (buffer_pos > 0) {
                    word_count++;
                    buffer_pos = 0;
                }
                current_state = IDLE;
                break;
                
            case WRITING:
                printf("\nStatistics:\n");
                printf("Words: %d\n", word_count);
                printf("Characters: %d\n", char_count);
                return 0;
                
            case ERROR:
                printf("Error: Invalid input detected\n");
                return 1;
        }
        
        if (ferror(stdin)) {
            printf("Error: Input stream error\n");
            return 1;
        }
    }
    
    return 0;
}
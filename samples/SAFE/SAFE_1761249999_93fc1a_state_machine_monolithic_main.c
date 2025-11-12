//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum state current_state = IDLE;
    char buffer[256];
    int buffer_index = 0;
    int total_numbers = 0;
    int total_words = 0;
    int ch;
    
    printf("Enter text (press Ctrl+D to finish):\n");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case IDLE:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        total_numbers++;
                        buffer_index = 0;
                    }
                    current_state = DONE;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        total_words++;
                        buffer_index = 0;
                    }
                    current_state = DONE;
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
            continue;
        }
        
        if (ch == '\n' || ch == ' ' || ch == '\t') {
            switch (current_state) {
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        total_numbers++;
                        buffer_index = 0;
                    }
                    current_state = IDLE;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        total_words++;
                        buffer_index = 0;
                    }
                    current_state = IDLE;
                    break;
                case IDLE:
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
            continue;
        }
        
        if (isdigit(ch)) {
            switch (current_state) {
                case IDLE:
                    current_state = READING_NUMBER;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_NUMBER:
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
            continue;
        }
        
        if (isalpha(ch)) {
            switch (current_state) {
                case IDLE:
                    current_state = READING_WORD;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_NUMBER:
                    current_state = ERROR;
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
            continue;
        }
        
        current_state = ERROR;
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format detected.\n");
        return 1;
    }
    
    printf("Analysis complete.\n");
    printf("Total numbers found: %d\n", total_numbers);
    printf("Total words found: %d\n", total_words);
    
    return 0;
}
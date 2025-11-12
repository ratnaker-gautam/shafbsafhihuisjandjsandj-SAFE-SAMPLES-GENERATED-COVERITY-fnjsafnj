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
    
    printf("Enter text (Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case IDLE:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    total_numbers++;
                    current_state = DONE;
                    break;
                case READING_WORD:
                    total_words++;
                    current_state = DONE;
                    break;
                default:
                    current_state = DONE;
                    break;
            }
            continue;
        }
        
        if (buffer_index >= 255) {
            current_state = ERROR;
            continue;
        }
        
        switch (current_state) {
            case IDLE:
                if (isspace(ch)) {
                    continue;
                } else if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_WORD;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    total_numbers++;
                    buffer_index = 0;
                    current_state = IDLE;
                } else if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    total_words++;
                    buffer_index = 0;
                    current_state = IDLE;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Input too long or invalid format\n");
        return 1;
    }
    
    printf("Total numbers: %d\n", total_numbers);
    printf("Total words: %d\n", total_words);
    
    return 0;
}
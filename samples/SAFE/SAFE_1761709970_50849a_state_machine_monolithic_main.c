//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int total_numbers = 0;
    int total_words = 0;
    int ch;
    
    printf("Enter text (max 255 chars, end with Ctrl+D): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case START:
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
        
        buffer[buffer_index++] = (char)ch;
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    buffer_index = 0;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isspace(ch)) {
                    total_numbers++;
                    buffer_index = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch)) {
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    total_words++;
                    buffer_index = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format detected.\n");
                buffer_index = 0;
                current_state = START;
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Processing terminated due to invalid input.\n");
        return 1;
    }
    
    printf("Total numbers: %d\n", total_numbers);
    printf("Total words: %d\n", total_words);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    enum State current_state = INIT;
    char buffer[256];
    int buffer_index = 0;
    int total_numbers = 0;
    int total_words = 0;
    int c;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        c = getchar();
        
        if (c == EOF) {
            if (current_state == READING_NUMBER) {
                total_numbers++;
                current_state = DONE;
            } else if (current_state == READING_WORD) {
                total_words++;
                current_state = DONE;
            } else if (current_state == INIT) {
                current_state = DONE;
            }
            continue;
        }
        
        if (buffer_index >= 255) {
            current_state = ERROR;
            continue;
        }
        
        switch (current_state) {
            case INIT:
                if (isdigit(c)) {
                    buffer[buffer_index++] = (char)c;
                    current_state = READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[buffer_index++] = (char)c;
                    current_state = READING_WORD;
                } else if (isspace(c)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    buffer[buffer_index++] = (char)c;
                } else if (isspace(c)) {
                    buffer[buffer_index] = '\0';
                    total_numbers++;
                    buffer_index = 0;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    buffer[buffer_index++] = (char)c;
                } else if (isspace(c)) {
                    buffer[buffer_index] = '\0';
                    total_words++;
                    buffer_index = 0;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format detected\n");
        return 1;
    }
    
    printf("Total numbers: %d\n", total_numbers);
    printf("Total words: %d\n", total_words);
    
    return 0;
}
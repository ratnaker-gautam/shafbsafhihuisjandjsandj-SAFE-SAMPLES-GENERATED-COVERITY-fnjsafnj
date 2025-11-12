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
    int ch;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while ((ch = getchar()) != EOF && buffer_index < 255) {
        if (current_state == ERROR) {
            break;
        }
        
        switch (current_state) {
            case INIT:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    buffer[buffer_index++] = (char)ch;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    buffer[buffer_index++] = (char)ch;
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    total_numbers++;
                    printf("Found number: %s\n", buffer);
                    buffer_index = 0;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    total_words++;
                    printf("Found word: %s\n", buffer);
                    buffer_index = 0;
                    current_state = INIT;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state == READING_NUMBER && buffer_index > 0) {
        buffer[buffer_index] = '\0';
        total_numbers++;
        printf("Found number: %s\n", buffer);
    } else if (current_state == READING_WORD && buffer_index > 0) {
        buffer[buffer_index] = '\0';
        total_words++;
        printf("Found word: %s\n", buffer);
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("\nSummary: %d numbers, %d words\n", total_numbers, total_words);
    
    return 0;
}
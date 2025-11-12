//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    int number_value = 0;
    int in_number = 0;

    printf("Enter text (max 255 chars): ");
    
    while ((ch = getchar()) != EOF && ch != '\n' && buffer_index < 255) {
        if (current_state == ERROR) break;
        
        buffer[buffer_index++] = (char)ch;
        
        switch (current_state) {
            case INIT:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    number_value = ch - '0';
                    in_number = 1;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (number_value <= (INT_MAX - (ch - '0')) / 10) {
                        number_value = number_value * 10 + (ch - '0');
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(ch) || ch == '_') {
                    current_state = ERROR;
                } else {
                    if (in_number) {
                        total_numbers++;
                        in_number = 0;
                    }
                    if (isalpha(ch)) {
                        current_state = READING_WORD;
                    } else if (isspace(ch)) {
                        current_state = INIT;
                    } else {
                        current_state = ERROR;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch) || ch == '_') {
                    break;
                } else if (isdigit(ch)) {
                    current_state = ERROR;
                } else if (isspace(ch)) {
                    total_words++;
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

    if (buffer_index < 256) {
        buffer[buffer_index] = '\0';
    } else {
        buffer[255] = '\0';
    }
    
    if (current_state != ERROR) {
        if (current_state == READING_NUMBER && in_number) {
            total_numbers++;
        } else if (current_state == READING_WORD) {
            total_words++;
        }
        current_state = DONE;
    }

    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Input: %s\n", buffer);
    printf("Total numbers: %d\n", total_numbers);
    printf("Total words: %d\n", total_words);
    
    return 0;
}
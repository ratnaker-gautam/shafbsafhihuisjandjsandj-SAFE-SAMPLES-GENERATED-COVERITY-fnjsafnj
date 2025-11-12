//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

int main(void) {
    enum state current_state = INIT;
    char buffer[256];
    int buffer_index = 0;
    int word_count = 0;
    int number_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != FINISHED && current_state != ERROR) {
        if (buffer_index >= 255) {
            current_state = FINISHED;
            break;
        }
        
        ch = getchar();
        if (ch == EOF || ch == '\n') {
            ch = ' ';
        }
        
        switch (current_state) {
            case INIT:
                if (ch == ' ') {
                    current_state = SKIPPING_SPACE;
                } else if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_WORD;
                } else if (ch != ' ') {
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
                } else if (ch == ' ' || ch == '\n') {
                    buffer[buffer_index] = '\0';
                    number_count++;
                    buffer_index = 0;
                    current_state = SKIPPING_SPACE;
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
                } else if (ch == ' ' || ch == '\n') {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    buffer_index = 0;
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (ch == ' ') {
                    break;
                } else if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_WORD;
                } else if (ch == '\n') {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (buffer_index > 0) {
        if (isdigit(buffer[0])) {
            number_count++;
        } else if (isalpha(buffer[0])) {
            word_count++;
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}
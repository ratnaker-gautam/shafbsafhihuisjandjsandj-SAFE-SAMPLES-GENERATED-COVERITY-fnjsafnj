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
    char buffer[256];
    int buffer_pos = 0;
    enum state current_state = INIT;
    int number_count = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != FINISHED && current_state != ERROR) {
        if (buffer_pos >= 255) {
            current_state = FINISHED;
            break;
        }
        
        ch = getchar();
        if (ch == EOF || ch == '\n') {
            ch = ' ';
        }
        
        switch (current_state) {
            case INIT:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace(ch)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isspace(ch)) {
                    current_state = SKIPPING_SPACE;
                } else if (isalpha(ch)) {
                    current_state = ERROR;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    current_state = SKIPPING_SPACE;
                } else if (isdigit(ch)) {
                    current_state = ERROR;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isspace(ch)) {
                    current_state = SKIPPING_SPACE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = FINISHED;
                }
                break;
                
            case FINISHED:
                break;
                
            case ERROR:
                break;
        }
        
        if (buffer_pos < 255 && current_state != ERROR && current_state != FINISHED) {
            buffer[buffer_pos++] = (char)ch;
        }
    }
    
    buffer[buffer_pos] = '\0';
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Processed text: %s\n", buffer);
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}
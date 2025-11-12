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
    int buffer_pos = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;
    
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
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    int num = atoi(buffer);
                    if (num > 0) {
                        number_sum += num;
                    }
                    buffer_pos = 0;
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    if (strlen(buffer) > 0) {
                        word_count++;
                    }
                    buffer_pos = 0;
                    current_state = SKIPPING_SPACE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else if (ch == ' ' && buffer_pos == 0) {
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
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        buffer[buffer_pos] = '\0';
        if (current_state == READING_NUMBER) {
            int num = atoi(buffer);
            if (num > 0) {
                number_sum += num;
            }
        } else {
            if (strlen(buffer) > 0) {
                word_count++;
            }
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
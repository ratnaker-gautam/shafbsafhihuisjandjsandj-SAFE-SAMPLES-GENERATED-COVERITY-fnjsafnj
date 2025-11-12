//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    enum state current_state = STATE_START;
    char buffer[256];
    int buffer_index = 0;
    int number_sum = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars, end with '.'): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = getchar();
        if (ch == EOF) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                    current_state = STATE_READING_WORD;
                } else if (ch == '.') {
                    current_state = STATE_END;
                } else if (!isspace(ch)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                    }
                    current_state = STATE_START;
                } else if (ch == '.') {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                    }
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalnum(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    current_state = STATE_START;
                } else if (ch == '.') {
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Number sum: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
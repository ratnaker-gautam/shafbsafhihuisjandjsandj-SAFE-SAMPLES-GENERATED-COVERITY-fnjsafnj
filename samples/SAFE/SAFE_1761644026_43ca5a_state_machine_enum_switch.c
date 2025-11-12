//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    enum state current_state = STATE_START;
    char buffer[256];
    int buffer_pos = 0;
    int word_count = 0;
    int number_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        if (buffer_pos >= 255) {
            current_state = STATE_END;
            break;
        }
        
        ch = getchar();
        if (ch == EOF || ch == '\n') {
            current_state = STATE_END;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = STATE_READING_WORD;
                } else if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = STATE_READING_NUMBER;
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    buffer_pos = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    buffer_pos = 0;
                    current_state = STATE_START;
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
    
    if (current_state == STATE_READING_WORD) {
        buffer[buffer_pos] = '\0';
        word_count++;
    } else if (current_state == STATE_READING_NUMBER) {
        buffer[buffer_pos] = '\0';
        number_count++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}
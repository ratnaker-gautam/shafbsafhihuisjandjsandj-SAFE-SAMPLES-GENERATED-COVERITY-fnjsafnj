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
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            ch = '\n';
        }
        
        switch (current_state) {
            case STATE_START:
                if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_READING_WORD;
                } else if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (ch == '\n') {
                    current_state = STATE_END;
                } else if (!isspace(ch)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalnum(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                } else if (isspace(ch) || ch == '\n') {
                    if (buffer_pos > 0 && buffer_pos <= 255) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        printf("Word: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    if (ch == '\n') {
                        current_state = STATE_END;
                    } else {
                        current_state = STATE_START;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                } else if (isspace(ch) || ch == '\n') {
                    if (buffer_pos > 0 && buffer_pos <= 255) {
                        buffer[buffer_pos] = '\0';
                        number_count++;
                        printf("Number: %s\n", buffer);
                    }
                    buffer_pos = 0;
                    if (ch == '\n') {
                        current_state = STATE_END;
                    } else {
                        current_state = STATE_START;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input character\n");
                buffer_pos = 0;
                if (ch == '\n') {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_END:
                break;
        }
    }
    
    printf("Words: %d, Numbers: %d\n", word_count, number_count);
    
    return 0;
}
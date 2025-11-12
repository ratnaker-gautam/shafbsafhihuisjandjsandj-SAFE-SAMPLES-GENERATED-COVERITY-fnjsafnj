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
    int buffer_pos = 0;
    int ch;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            ch = '\n';
        }
        
        if (buffer_pos >= 255) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = STATE_READING_WORD;
                } else if (ch == '\n') {
                    current_state = STATE_END;
                } else if (!isspace(ch)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (isspace(ch) || ch == '\n') {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
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
                
            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (isspace(ch) || ch == '\n') {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
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
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (buffer_pos > 0) {
        if (current_state == STATE_READING_NUMBER) {
            buffer[buffer_pos] = '\0';
            number_count++;
            printf("Found number: %s\n", buffer);
        } else if (current_state == STATE_READING_WORD) {
            buffer[buffer_pos] = '\0';
            word_count++;
            printf("Found word: %s\n", buffer);
        }
    }
    
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    
    return 0;
}
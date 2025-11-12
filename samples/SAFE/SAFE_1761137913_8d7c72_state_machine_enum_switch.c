//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING,
    STATE_NUMBER,
    STATE_WORD,
    STATE_END,
    STATE_ERROR
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
                if (ch == '\n') {
                    current_state = STATE_END;
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                } else if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_NUMBER;
                } else if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING:
                if (ch == '\n') {
                    current_state = STATE_END;
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                } else if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_NUMBER;
                } else if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_NUMBER:
                if (ch == '\n') {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                        buffer_pos = 0;
                    }
                    current_state = STATE_END;
                } else if (isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        number_count++;
                        printf("Found number: %s\n", buffer);
                        buffer_pos = 0;
                    }
                    current_state = STATE_START;
                } else if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_NUMBER;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_WORD:
                if (ch == '\n') {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                        buffer_pos = 0;
                    }
                    current_state = STATE_END;
                } else if (isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        word_count++;
                        printf("Found word: %s\n", buffer);
                        buffer_pos = 0;
                    }
                    current_state = STATE_START;
                } else if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    }
                    current_state = STATE_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_END:
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input character\n");
                return 1;
        }
    }
    
    if (current_state == STATE_END) {
        printf("\nSummary: %d words, %d numbers\n", word_count, number_count);
    }
    
    return 0;
}
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
    int number_count = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        if (buffer_index >= 255) {
            current_state = STATE_ERROR;
            break;
        }
        
        ch = getchar();
        if (ch == EOF || ch == '\n') {
            ch = ' ';
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    buffer[buffer_index++] = (char)ch;
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    buffer[buffer_index++] = (char)ch;
                } else if (ch == ' ') {
                    continue;
                } else {
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
                } else if (ch == ' ') {
                    buffer[buffer_index] = '\0';
                    number_count++;
                    printf("Found number: %s\n", buffer);
                    buffer_index = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (ch == ' ') {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    printf("Found word: %s\n", buffer);
                    buffer_index = 0;
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
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        if (buffer_index > 0 && buffer_index < 256) {
            buffer[buffer_index] = '\0';
            if (current_state == STATE_READING_NUMBER) {
                number_count++;
                printf("Found number: %s\n", buffer);
            } else {
                word_count++;
                printf("Found word: %s\n", buffer);
            }
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}
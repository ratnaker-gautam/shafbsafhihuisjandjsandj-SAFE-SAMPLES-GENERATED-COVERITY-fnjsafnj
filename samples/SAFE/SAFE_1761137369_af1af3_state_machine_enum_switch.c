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
    STATE_END
};

int main(void) {
    enum state current_state = STATE_START;
    char buffer[256];
    int buffer_index = 0;
    int word_count = 0;
    int number_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != STATE_END && buffer_index < 255) {
        ch = getchar();
        
        if (ch == EOF) {
            ch = '\n';
        }
        
        switch (current_state) {
            case STATE_START:
                if (ch == '\n') {
                    current_state = STATE_END;
                } else if (isdigit(ch)) {
                    current_state = STATE_NUMBER;
                    buffer[buffer_index++] = (char)ch;
                } else if (isalpha(ch)) {
                    current_state = STATE_WORD;
                    buffer[buffer_index++] = (char)ch;
                } else if (!isspace(ch)) {
                    current_state = STATE_READING;
                    buffer[buffer_index++] = (char)ch;
                }
                break;
                
            case STATE_READING:
                if (ch == '\n' || isspace(ch)) {
                    current_state = STATE_START;
                    buffer[buffer_index] = '\0';
                    printf("Other: %s\n", buffer);
                    buffer_index = 0;
                } else {
                    buffer[buffer_index++] = (char)ch;
                }
                break;
                
            case STATE_NUMBER:
                if (ch == '\n' || isspace(ch)) {
                    current_state = STATE_START;
                    buffer[buffer_index] = '\0';
                    number_count++;
                    printf("Number %d: %s\n", number_count, buffer);
                    buffer_index = 0;
                } else if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = STATE_READING;
                    buffer[buffer_index++] = (char)ch;
                }
                break;
                
            case STATE_WORD:
                if (ch == '\n' || isspace(ch)) {
                    current_state = STATE_START;
                    buffer[buffer_index] = '\0';
                    word_count++;
                    printf("Word %d: %s\n", word_count, buffer);
                    buffer_index = 0;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = STATE_READING;
                    buffer[buffer_index++] = (char)ch;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (buffer_index >= 255) {
            buffer[255] = '\0';
            printf("Buffer full: %s\n", buffer);
            buffer_index = 0;
        }
    }
    
    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        if (current_state == STATE_NUMBER) {
            number_count++;
            printf("Number %d: %s\n", number_count, buffer);
        } else if (current_state == STATE_WORD) {
            word_count++;
            printf("Word %d: %s\n", word_count, buffer);
        } else {
            printf("Other: %s\n", buffer);
        }
    }
    
    printf("\nSummary: %d words, %d numbers\n", word_count, number_count);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    char buffer[256];
    int buffer_pos = 0;
    enum State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            if (current_state == STATE_READING_NUMBER) {
                number_count++;
            } else if (current_state == STATE_READING_WORD) {
                word_count++;
            }
            current_state = STATE_END;
            continue;
        }
        
        if (buffer_pos >= 255) {
            current_state = STATE_ERROR;
            continue;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = STATE_READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    number_count++;
                    buffer_pos = 0;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalnum(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (isspace(ch)) {
                    buffer[buffer_pos] = '\0';
                    word_count++;
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
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format detected.\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}
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
    STATE_DONE
};

int main(void) {
    char buffer[256];
    int buffer_pos = 0;
    enum State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars, Ctrl+D to end): ");
    
    while ((ch = getchar()) != EOF && buffer_pos < 255) {
        if (ch == '\n') {
            buffer[buffer_pos] = '\0';
            break;
        }
        buffer[buffer_pos++] = (char)ch;
    }
    
    if (buffer_pos == 255) {
        buffer[255] = '\0';
    } else if (buffer_pos == 0) {
        buffer[0] = '\0';
    }
    
    buffer_pos = 0;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        ch = buffer[buffer_pos];
        
        switch (current_state) {
            case STATE_START:
                if (ch == '\0') {
                    current_state = STATE_DONE;
                } else if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isspace(ch)) {
                    buffer_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (ch == '\0') {
                    current_state = STATE_DONE;
                } else if (isdigit(ch)) {
                    buffer_pos++;
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                    buffer_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (ch == '\0') {
                    current_state = STATE_DONE;
                } else if (isalpha(ch)) {
                    buffer_pos++;
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                    buffer_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}
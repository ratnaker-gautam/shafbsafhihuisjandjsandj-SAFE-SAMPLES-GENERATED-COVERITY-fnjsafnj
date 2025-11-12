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
    
    printf("Enter text (max 255 chars): ");
    
    while ((ch = getchar()) != EOF && buffer_pos < 255) {
        if (ch == '\n') {
            break;
        }
        buffer[buffer_pos++] = (char)ch;
    }
    
    if (buffer_pos >= 255) {
        printf("Input too long\n");
        return 1;
    }
    
    buffer[buffer_pos] = '\0';
    buffer_pos = 0;
    
    while (buffer[buffer_pos] != '\0') {
        switch (current_state) {
            case STATE_START:
                if (isdigit(buffer[buffer_pos])) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (isalpha(buffer[buffer_pos])) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (!isspace(buffer[buffer_pos])) {
                    current_state = STATE_ERROR;
                }
                buffer_pos++;
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(buffer[buffer_pos])) {
                    buffer_pos++;
                } else if (isspace(buffer[buffer_pos])) {
                    current_state = STATE_START;
                    buffer_pos++;
                } else if (isalpha(buffer[buffer_pos])) {
                    current_state = STATE_ERROR;
                } else {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(buffer[buffer_pos])) {
                    buffer_pos++;
                } else if (isspace(buffer[buffer_pos])) {
                    current_state = STATE_START;
                    buffer_pos++;
                } else if (isdigit(buffer[buffer_pos])) {
                    current_state = STATE_ERROR;
                } else {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        current_state = STATE_DONE;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}
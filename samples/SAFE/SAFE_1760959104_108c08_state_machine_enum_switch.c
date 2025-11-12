//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
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
    char buffer[256];
    enum state current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = buffer[pos];
        
        switch (current_state) {
            case STATE_START:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isspace(ch)) {
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isdigit(ch)) {
                    pos++;
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isalpha(ch)) {
                    pos++;
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid character at position %d\n", pos);
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Processing failed due to invalid input\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}
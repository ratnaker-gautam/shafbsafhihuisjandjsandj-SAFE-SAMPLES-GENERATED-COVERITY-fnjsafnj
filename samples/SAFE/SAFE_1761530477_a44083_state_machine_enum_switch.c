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
    char input[256];
    int word_count = 0;
    int number_count = 0;
    int pos = 0;
    char ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    while (pos < len && current_state != STATE_ERROR && current_state != STATE_END) {
        ch = input[pos];
        
        switch (current_state) {
            case STATE_START:
                if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (ch == ' ' || ch == '\t') {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                } else if (ch == ' ' || ch == '\t') {
                    current_state = STATE_START;
                } else if (ch == '\0') {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                } else if (ch == ' ' || ch == '\t') {
                    current_state = STATE_START;
                } else if (ch == '\0') {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_END:
                break;
        }
        
        pos++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format at position %d\n", pos);
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}
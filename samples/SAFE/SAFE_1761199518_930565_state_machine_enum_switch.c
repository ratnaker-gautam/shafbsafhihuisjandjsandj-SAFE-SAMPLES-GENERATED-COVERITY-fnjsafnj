//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
};

int main(void) {
    char input[256];
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    enum state current_state = STATE_START;
    size_t pos = 0;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char c = (pos < len) ? input[pos] : '\0';
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (isspace(c)) {
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isalpha(c)) {
                    pos++;
                } else if (isspace(c)) {
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isdigit(c)) {
                    pos++;
                } else if (isspace(c)) {
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_END:
                break;
                
            case STATE_ERROR:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}
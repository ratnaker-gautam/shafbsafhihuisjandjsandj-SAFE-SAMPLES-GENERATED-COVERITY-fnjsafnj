//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
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
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    enum state current_state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    size_t pos = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR && pos <= len) {
        char c = (pos < len) ? input[pos] : '\0';
        
        switch (current_state) {
            case STATE_START:
                if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                pos++;
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    pos++;
                } else if (c == ' ' || c == '\t' || c == '\0') {
                    current_state = STATE_START;
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    pos++;
                } else if (c == ' ' || c == '\t' || c == '\0') {
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
        printf("Invalid input format at position %zu\n", pos);
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}
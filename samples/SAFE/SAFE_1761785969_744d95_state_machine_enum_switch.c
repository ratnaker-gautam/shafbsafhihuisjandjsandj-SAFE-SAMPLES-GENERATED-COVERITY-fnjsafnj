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
    char buffer[256];
    printf("Enter text (max 255 chars): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    enum state current_state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    size_t pos = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR && pos <= len) {
        char c = (pos < len) ? buffer[pos] : '\0';
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isalpha((unsigned char)c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isdigit((unsigned char)c)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (!isspace((unsigned char)c)) {
                    current_state = STATE_ERROR;
                }
                pos++;
                break;
                
            case STATE_READING_WORD:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isspace((unsigned char)c)) {
                    current_state = STATE_START;
                    pos++;
                } else if (!isalpha((unsigned char)c)) {
                    current_state = STATE_ERROR;
                } else {
                    pos++;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isspace((unsigned char)c)) {
                    current_state = STATE_START;
                    pos++;
                } else if (!isdigit((unsigned char)c)) {
                    current_state = STATE_ERROR;
                } else {
                    pos++;
                }
                break;
                
            case STATE_END:
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
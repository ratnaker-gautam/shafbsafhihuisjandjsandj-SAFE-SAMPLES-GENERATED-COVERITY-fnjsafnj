//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    char buffer[256];
    enum State current_state = STATE_START;
    int number_sum = 0;
    int word_count = 0;
    int pos = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = buffer[pos];
        
        switch (current_state) {
            case STATE_START:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                } else if (isspace(ch)) {
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    int digit = ch - '0';
                    if (number_sum > (INT_MAX - digit) / 10) {
                        current_state = STATE_ERROR;
                        break;
                    }
                    number_sum = number_sum * 10 + digit;
                    pos++;
                } else if (isspace(ch) || ch == '\0') {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(ch)) {
                    pos++;
                } else if (isspace(ch) || ch == '\0') {
                    word_count++;
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
        
        if (pos >= 255) {
            current_state = STATE_END;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (current_state == STATE_READING_NUMBER) {
        current_state = STATE_END;
    }
    
    if (current_state == STATE_READING_WORD) {
        word_count++;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
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
    enum state current_state = STATE_START;
    char input[256];
    int word_count = 0;
    int number_count = 0;
    int pos = 0;
    char ch;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = input[pos];
        
        switch (current_state) {
            case STATE_START:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isalpha((unsigned char)ch)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isdigit((unsigned char)ch)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (isspace((unsigned char)ch)) {
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isalpha((unsigned char)ch)) {
                    pos++;
                } else if (isspace((unsigned char)ch)) {
                    current_state = STATE_START;
                    pos++;
                } else if (isdigit((unsigned char)ch)) {
                    current_state = STATE_ERROR;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isdigit((unsigned char)ch)) {
                    pos++;
                } else if (isspace((unsigned char)ch)) {
                    current_state = STATE_START;
                    pos++;
                } else if (isalpha((unsigned char)ch)) {
                    current_state = STATE_ERROR;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_END:
                break;
                
            case STATE_ERROR:
                break;
        }
        
        if (pos >= 255) {
            current_state = STATE_END;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format at position %d\n", pos);
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}
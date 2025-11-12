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
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = input[pos];
        
        switch (current_state) {
            case STATE_START:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (isspace(ch)) {
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
                
            case STATE_ERROR:
                printf("Invalid input format at position %d\n", pos);
                break;
                
            case STATE_END:
                break;
        }
        
        if (pos >= 255) {
            current_state = STATE_END;
        }
    }
    
    if (current_state == STATE_END) {
        printf("Words: %d, Numbers: %d\n", word_count, number_count);
    } else {
        printf("Processing failed\n");
        return 1;
    }
    
    return 0;
}
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
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    int word_count = 0;
    int number_count = 0;
    int current_pos = 0;
    enum state current_state = STATE_START;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        char c = input[current_pos];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0' || c == '\n') {
                    current_state = STATE_END;
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (isspace(c)) {
                    current_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || c == '\n') {
                    current_state = STATE_END;
                } else if (isalpha(c)) {
                    current_pos++;
                } else if (isspace(c)) {
                    current_state = STATE_START;
                    current_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || c == '\n') {
                    current_state = STATE_END;
                } else if (isdigit(c)) {
                    current_pos++;
                } else if (isspace(c)) {
                    current_state = STATE_START;
                    current_pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_END:
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
        }
    }

    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}
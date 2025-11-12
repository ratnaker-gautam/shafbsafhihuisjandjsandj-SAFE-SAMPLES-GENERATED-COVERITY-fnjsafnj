//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
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
    enum state current_state = STATE_START;
    char buffer[256];
    int buffer_index = 0;
    int number_sum = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (Ctrl+D to end): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = getchar();
        
        switch (current_state) {
            case STATE_START:
                if (ch == EOF) {
                    current_state = STATE_END;
                } else if (isdigit(ch)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                    current_state = STATE_READING_WORD;
                } else if (isspace(ch)) {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (ch == EOF || isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        char *endptr;
                        long num = strtol(buffer, &endptr, 10);
                        if (endptr != buffer && *endptr == '\0' && num >= 0 && num <= 1000000) {
                            number_sum += (int)num;
                        }
                    }
                    if (ch == EOF) {
                        current_state = STATE_END;
                    } else {
                        current_state = STATE_START;
                    }
                } else if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (ch == EOF || isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    if (ch == EOF) {
                        current_state = STATE_END;
                    } else {
                        current_state = STATE_START;
                    }
                } else if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                current_state = STATE_END;
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_END) {
        printf("Number sum: %d\n", number_sum);
        printf("Word count: %d\n", word_count);
    }
    
    return current_state == STATE_ERROR ? 1 : 0;
}
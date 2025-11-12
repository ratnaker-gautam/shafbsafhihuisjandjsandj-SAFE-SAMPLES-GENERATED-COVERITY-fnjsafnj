//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    enum State current_state = STATE_START;
    char buffer[256];
    int buffer_index = 0;
    int number_sum = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = getchar();
        
        if (ch == EOF || ch == '\n') {
            if (current_state == STATE_READING_NUMBER) {
                if (buffer_index > 0) {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    buffer_index = 0;
                }
            } else if (current_state == STATE_READING_WORD) {
                if (buffer_index > 0) {
                    word_count++;
                    buffer_index = 0;
                }
            }
            current_state = STATE_END;
            continue;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (!isspace(ch)) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        buffer_index = 0;
                    }
                    current_state = STATE_START;
                } else if (isalpha(ch)) {
                    current_state = STATE_ERROR;
                } else {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                }
                break;
                
            case STATE_READING_WORD:
                if (isalnum(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                    }
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
        
        if (current_state == STATE_ERROR) {
            printf("Error: Invalid input format\n");
            break;
        }
    }
    
    if (current_state == STATE_END) {
        printf("Number sum: %d\n", number_sum);
        printf("Word count: %d\n", word_count);
    }
    
    return current_state == STATE_ERROR ? 1 : 0;
}
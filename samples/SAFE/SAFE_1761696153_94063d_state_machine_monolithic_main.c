//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    enum State current_state = INIT;
    char buffer[256];
    int buffer_pos = 0;
    int number_sum = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (numbers and words, end with Ctrl+D):\n");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case INIT:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        buffer_pos = 0;
                    }
                    current_state = DONE;
                    break;
                case READING_WORD:
                    if (buffer_pos > 0) {
                        word_count++;
                        buffer_pos = 0;
                    }
                    current_state = DONE;
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
            continue;
        }
        
        if (ch == '\n' || ch == ' ' || ch == '\t') {
            switch (current_state) {
                case READING_NUMBER:
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        buffer_pos = 0;
                    }
                    current_state = INIT;
                    break;
                case READING_WORD:
                    if (buffer_pos > 0) {
                        word_count++;
                        buffer_pos = 0;
                    }
                    current_state = INIT;
                    break;
                case INIT:
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
        } else if (isdigit(ch)) {
            switch (current_state) {
                case INIT:
                case READING_NUMBER:
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = ch;
                        current_state = READING_NUMBER;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_WORD:
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
        } else if (isalpha(ch)) {
            switch (current_state) {
                case INIT:
                case READING_WORD:
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = tolower(ch);
                        current_state = READING_WORD;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_NUMBER:
                    current_state = ERROR;
                    break;
                default:
                    current_state = ERROR;
                    break;
            }
        } else {
            current_state = ERROR;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Number sum: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
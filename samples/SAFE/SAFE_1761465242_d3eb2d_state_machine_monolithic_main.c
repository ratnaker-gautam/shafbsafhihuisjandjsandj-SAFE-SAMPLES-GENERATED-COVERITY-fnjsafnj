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
    int buffer_index = 0;
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
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        current_state = DONE;
                    } else {
                        current_state = DONE;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        current_state = DONE;
                    } else {
                        current_state = DONE;
                    }
                    break;
                default:
                    current_state = DONE;
            }
            continue;
        }
        
        if (ch == '\n') {
            switch (current_state) {
                case INIT:
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        buffer_index = 0;
                        current_state = INIT;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                        current_state = INIT;
                    }
                    break;
                default:
                    current_state = INIT;
            }
            continue;
        }
        
        if (isspace(ch)) {
            switch (current_state) {
                case INIT:
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        buffer_index = 0;
                        current_state = INIT;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                        current_state = INIT;
                    }
                    break;
                default:
                    current_state = INIT;
            }
            continue;
        }
        
        if (current_state == INIT) {
            if (isdigit(ch)) {
                current_state = READING_NUMBER;
                if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
            } else if (isalpha(ch)) {
                current_state = READING_WORD;
                if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
            } else {
                current_state = ERROR;
            }
        } else if (current_state == READING_NUMBER) {
            if (isdigit(ch)) {
                if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
            } else if (isalpha(ch) || ch == '.') {
                current_state = ERROR;
            } else {
                current_state = ERROR;
            }
        } else if (current_state == READING_WORD) {
            if (isalnum(ch)) {
                if (buffer_index < 255) {
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
            } else {
                current_state = ERROR;
            }
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format detected.\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
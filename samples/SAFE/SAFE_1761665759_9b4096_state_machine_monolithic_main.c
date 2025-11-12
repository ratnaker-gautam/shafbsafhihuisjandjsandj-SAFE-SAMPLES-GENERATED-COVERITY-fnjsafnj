//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char buffer[1024];
    int buffer_pos = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 1023 chars, Ctrl+D to end): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case START:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        int num = atoi(buffer);
                        if (num > 0 && number_sum <= INT_MAX - num) {
                            number_sum += num;
                        }
                        current_state = DONE;
                    } else {
                        current_state = DONE;
                    }
                    break;
                case READING_WORD:
                    if (buffer_pos > 0) {
                        word_count++;
                        current_state = DONE;
                    } else {
                        current_state = DONE;
                    }
                    break;
                default:
                    current_state = DONE;
                    break;
            }
            continue;
        }
        
        if (ch == '\n') {
            switch (current_state) {
                case START:
                    break;
                case READING_NUMBER:
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        int num = atoi(buffer);
                        if (num > 0 && number_sum <= INT_MAX - num) {
                            number_sum += num;
                        }
                        buffer_pos = 0;
                        current_state = START;
                    }
                    break;
                case READING_WORD:
                    if (buffer_pos > 0) {
                        word_count++;
                        buffer_pos = 0;
                        current_state = START;
                    }
                    break;
                default:
                    break;
            }
            continue;
        }
        
        if (buffer_pos >= 1023) {
            current_state = ERROR;
            continue;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_WORD;
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (isalpha(ch) || ch == '.') {
                    current_state = ERROR;
                } else if (isspace(ch)) {
                    if (buffer_pos > 0) {
                        buffer[buffer_pos] = '\0';
                        int num = atoi(buffer);
                        if (num > 0 && number_sum <= INT_MAX - num) {
                            number_sum += num;
                        }
                        buffer_pos = 0;
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                } else if (isspace(ch)) {
                    if (buffer_pos > 0) {
                        word_count++;
                        buffer_pos = 0;
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
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
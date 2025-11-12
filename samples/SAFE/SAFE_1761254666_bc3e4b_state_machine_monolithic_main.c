//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars, end with Ctrl+D): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case START:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0 || (num == 0 && buffer[0] == '0')) {
                            number_sum += num;
                        }
                        current_state = DONE;
                    } else {
                        current_state = DONE;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    current_state = DONE;
                    break;
                default:
                    current_state = DONE;
            }
            continue;
        }
        
        if (ch == '\n') {
            switch (current_state) {
                case START:
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0 || (num == 0 && buffer[0] == '0')) {
                            number_sum += num;
                        }
                        buffer_index = 0;
                        current_state = START;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                        current_state = START;
                    }
                    break;
                default:
                    current_state = START;
            }
            continue;
        }
        
        if (isspace(ch)) {
            switch (current_state) {
                case START:
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        if (num > 0 || (num == 0 && buffer[0] == '0')) {
                            number_sum += num;
                        }
                        buffer_index = 0;
                        current_state = START;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                        current_state = START;
                    }
                    break;
                default:
                    current_state = START;
            }
            continue;
        }
        
        if (isdigit(ch)) {
            switch (current_state) {
                case START:
                    current_state = READING_NUMBER;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_NUMBER:
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                default:
                    current_state = ERROR;
            }
            continue;
        }
        
        if (isalpha(ch)) {
            switch (current_state) {
                case START:
                    current_state = READING_WORD;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_NUMBER:
                    current_state = READING_WORD;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                case READING_WORD:
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                    break;
                default:
                    current_state = ERROR;
            }
            continue;
        }
        
        current_state = ERROR;
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input or buffer overflow\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
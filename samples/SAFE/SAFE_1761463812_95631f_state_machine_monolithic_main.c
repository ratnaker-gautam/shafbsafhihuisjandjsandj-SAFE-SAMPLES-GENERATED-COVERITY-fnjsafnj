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
    
    printf("Enter text (max 255 chars, end with EOF or newline): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF || ch == '\n') {
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
                        buffer_index = 0;
                    }
                    current_state = DONE;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                    }
                    current_state = DONE;
                    break;
                default:
                    break;
            }
            continue;
        }
        
        if (buffer_index >= 255) {
            current_state = ERROR;
            continue;
        }
        
        if (isspace(ch)) {
            switch (current_state) {
                case READING_NUMBER:
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    if (num > 0 || (num == 0 && buffer[0] == '0')) {
                        number_sum += num;
                    }
                    buffer_index = 0;
                    current_state = START;
                    break;
                case READING_WORD:
                    word_count++;
                    buffer_index = 0;
                    current_state = START;
                    break;
                default:
                    break;
            }
            continue;
        }
        
        if (isdigit(ch)) {
            switch (current_state) {
                case START:
                    current_state = READING_NUMBER;
                    buffer[buffer_index++] = (char)ch;
                    break;
                case READING_NUMBER:
                    buffer[buffer_index++] = (char)ch;
                    break;
                case READING_WORD:
                    buffer[buffer_index++] = (char)ch;
                    break;
                default:
                    break;
            }
        } else if (isalpha(ch)) {
            switch (current_state) {
                case START:
                    current_state = READING_WORD;
                    buffer[buffer_index++] = (char)ch;
                    break;
                case READING_NUMBER:
                    current_state = READING_WORD;
                    buffer[buffer_index++] = (char)ch;
                    break;
                case READING_WORD:
                    buffer[buffer_index++] = (char)ch;
                    break;
                default:
                    break;
            }
        } else {
            current_state = ERROR;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input detected\n");
        return 1;
    }
    
    printf("Word count: %d\n", word_count);
    printf("Number sum: %d\n", number_sum);
    
    return 0;
}
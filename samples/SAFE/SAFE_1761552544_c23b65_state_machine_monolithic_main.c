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
    
    printf("Enter text (max 255 chars): ");
    
    while ((ch = getchar()) != EOF && ch != '\n' && buffer_index < 255) {
        if (current_state == ERROR) break;
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_WORD;
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    buffer_index = 0;
                    current_state = START;
                } else if (isalpha(ch)) {
                    current_state = ERROR;
                } else {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    buffer_index = 0;
                    current_state = START;
                } else if (isdigit(ch)) {
                    current_state = ERROR;
                } else {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                }
                break;
                
            default:
                break;
        }
    }
    
    if (current_state == READING_NUMBER && buffer_index > 0) {
        buffer[buffer_index] = '\0';
        int num = atoi(buffer);
        number_sum += num;
    } else if (current_state == READING_WORD && buffer_index > 0) {
        word_count++;
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
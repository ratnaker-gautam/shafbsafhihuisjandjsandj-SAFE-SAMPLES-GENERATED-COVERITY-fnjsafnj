//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { IDLE, READING_NUMBER, READING_WORD, ERROR, DONE };

int main(void) {
    enum state current_state = IDLE;
    char buffer[256];
    int buffer_index = 0;
    int number_sum = 0;
    int word_count = 0;
    int ch;
    
    printf("Enter text (max 255 chars): ");
    
    while ((ch = getchar()) != EOF && ch != '\n' && buffer_index < 255) {
        if (current_state == ERROR) break;
        
        switch (current_state) {
            case IDLE:
                if (isspace(ch)) {
                    continue;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    buffer[buffer_index++] = (char)ch;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    buffer[buffer_index++] = (char)ch;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    buffer_index = 0;
                    current_state = IDLE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    buffer[buffer_index] = '\0';
                    word_count++;
                    buffer_index = 0;
                    current_state = IDLE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
    }
    
    if (current_state != ERROR) {
        if (current_state == READING_NUMBER && buffer_index > 0) {
            buffer[buffer_index] = '\0';
            int num = atoi(buffer);
            number_sum += num;
            current_state = DONE;
        } else if (current_state == READING_WORD && buffer_index > 0) {
            buffer[buffer_index] = '\0';
            word_count++;
            current_state = DONE;
        } else if (current_state == IDLE) {
            current_state = DONE;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
        buffer[buffer_index++] = (char)ch;
    }
    
    if (buffer_index >= 255) {
        printf("Input too long.\n");
        return 1;
    }
    
    buffer[buffer_index] = '\0';
    buffer_index = 0;
    
    while (current_state != DONE && current_state != ERROR) {
        ch = buffer[buffer_index];
        
        switch (current_state) {
            case START:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    buffer_index++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    buffer_index++;
                } else if (isspace(ch)) {
                    current_state = START;
                    buffer_index++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (ch == '\0') {
                    word_count++;
                    current_state = DONE;
                } else if (isalpha(ch)) {
                    buffer_index++;
                } else if (isspace(ch)) {
                    word_count++;
                    current_state = START;
                    buffer_index++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format.\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    buffer_index = 0;
    current_state = START;
    int current_number = 0;
    
    while (current_state != DONE && current_state != ERROR) {
        ch = buffer[buffer_index];
        
        switch (current_state) {
            case START:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    current_number = ch - '0';
                    current_state = READING_NUMBER;
                    buffer_index++;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    buffer_index++;
                } else if (isspace(ch)) {
                    buffer_index++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (ch == '\0') {
                    number_sum += current_number;
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    if (current_number <= (INT_MAX - (ch - '0')) / 10) {
                        current_number = current_number * 10 + (ch - '0');
                        buffer_index++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    number_sum += current_number;
                    current_state = START;
                    buffer_index++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (ch == '\0') {
                    current_state = DONE;
                } else if (isalpha(ch)) {
                    buffer_index++;
                } else if (isspace(ch)) {
                    current_state = START;
                    buffer_index++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Processing error.\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    printf("Word count: %d\n", word_count);
    printf("Number sum: %d\n", number_sum);
    
    return 0;
}
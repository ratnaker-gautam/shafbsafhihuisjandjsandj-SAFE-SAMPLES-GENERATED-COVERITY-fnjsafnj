//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (end with Ctrl+D): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        switch (current_state) {
            case START:
                if (ch == EOF) {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                    current_state = READING_NUM;
                } else if (isalpha(ch)) {
                    buffer_index = 0;
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)tolower(ch);
                    }
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (isspace(ch) || ch == EOF) {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    number_sum += num;
                    if (ch == EOF) {
                        current_state = DONE;
                    } else {
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)tolower(ch);
                    }
                } else if (isspace(ch) || ch == EOF) {
                    buffer[buffer_index] = '\0';
                    if (strlen(buffer) > 0) {
                        word_count++;
                    }
                    if (ch == EOF) {
                        current_state = DONE;
                    } else {
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == DONE) {
        printf("Sum of numbers: %d\n", number_sum);
        printf("Word count: %d\n", word_count);
    } else {
        printf("Processing error occurred\n");
        return 1;
    }
    
    return 0;
}
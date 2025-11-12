//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

int main(void) {
    enum State current_state = START;
    char buffer[256];
    int buffer_pos = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        switch (current_state) {
            case START:
                if (ch == EOF || ch == '\n') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_NUM;
                } else if (isalpha(ch)) {
                    buffer[buffer_pos++] = (char)ch;
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUM:
                if (isdigit(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch) || ch == EOF || ch == '\n') {
                    buffer[buffer_pos] = '\0';
                    number_sum += atoi(buffer);
                    buffer_pos = 0;
                    current_state = (ch == EOF || ch == '\n') ? DONE : START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_pos < 255) {
                        buffer[buffer_pos++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch) || ch == EOF || ch == '\n') {
                    buffer[buffer_pos] = '\0';
                    word_count++;
                    buffer_pos = 0;
                    current_state = (ch == EOF || ch == '\n') ? DONE : START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        if (buffer_pos >= 256) {
            current_state = ERROR;
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
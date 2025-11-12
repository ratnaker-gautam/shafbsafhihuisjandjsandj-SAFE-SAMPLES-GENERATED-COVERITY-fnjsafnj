//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    enum state current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;

    printf("Enter text (max 255 chars, end with EOF or newline): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF || ch == '\n') {
            ch = ' ';
        }

        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)tolower(ch);
                    current_state = READING_WORD;
                } else if (ch == ' ') {
                    current_state = START;
                } else if (ch == '\0') {
                    current_state = DONE;
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
                } else if (ch == ' ' || ch == '\0') {
                    buffer[buffer_index] = '\0';
                    int num = atoi(buffer);
                    if (num > 0) {
                        number_sum += num;
                    }
                    buffer_index = 0;
                    current_state = (ch == '\0') ? DONE : START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)tolower(ch);
                    } else {
                        current_state = ERROR;
                    }
                } else if (ch == ' ' || ch == '\0') {
                    buffer[buffer_index] = '\0';
                    if (strlen(buffer) > 0) {
                        word_count++;
                    }
                    buffer_index = 0;
                    current_state = (ch == '\0') ? DONE : START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Error: Invalid input format\n");
                current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }

    if (current_state == DONE) {
        printf("Number sum: %d\n", number_sum);
        printf("Word count: %d\n", word_count);
    }

    return (current_state == ERROR) ? 1 : 0;
}
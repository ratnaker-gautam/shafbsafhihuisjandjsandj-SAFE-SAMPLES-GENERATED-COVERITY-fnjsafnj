//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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

    printf("Enter text (max 255 chars, end with Ctrl+D): ");
    
    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            ch = '\n';
        }

        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    buffer_index = 0;
                    buffer[buffer_index++] = (char)ch;
                    current_state = READING_WORD;
                } else if (ch == '\n') {
                    current_state = DONE;
                } else if (!isspace(ch)) {
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
                } else if (isspace(ch) || ch == '\n') {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        int num = atoi(buffer);
                        number_sum += num;
                    }
                    if (ch == '\n') {
                        current_state = DONE;
                    } else {
                        current_state = START;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch) || ch == '\n') {
                    buffer[buffer_index] = '\0';
                    if (buffer_index > 0) {
                        word_count++;
                    }
                    if (ch == '\n') {
                        current_state = DONE;
                    } else {
                        current_state = START;
                    }
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

    return current_state == ERROR ? 1 : 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    enum State current_state = START;
    char buffer[256];
    int buffer_index = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    buffer_index = 0;
    while (buffer_index < sizeof(buffer) && buffer[buffer_index] != '\0') {
        ch = buffer[buffer_index];
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    number_sum += (ch - '0');
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (ch == '\n' || ch == '\0') {
                    current_state = DONE;
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    number_sum += (ch - '0');
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (ch == '\n' || ch == '\0') {
                    current_state = DONE;
                } else if (isspace(ch)) {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                } else if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    number_sum += (ch - '0');
                } else if (ch == '\n' || ch == '\0') {
                    current_state = DONE;
                } else if (isspace(ch)) {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                if (ch == '\n' || ch == '\0') {
                    current_state = DONE;
                } else if (isspace(ch)) {
                    current_state = START;
                }
                break;
                
            case DONE:
                break;
        }
        
        if (current_state == DONE) {
            break;
        }
        
        buffer_index++;
    }

    printf("Sum of digits: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
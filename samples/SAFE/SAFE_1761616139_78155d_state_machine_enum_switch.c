//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    while (buffer_index < sizeof(buffer) - 1 && buffer[buffer_index] != '\0') {
        ch = buffer[buffer_index];

        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    current_state = READING_NUMBER;
                    buffer_index--;
                } else if (isalpha(ch)) {
                    current_state = READING_WORD;
                    buffer_index--;
                } else if (ch == '\n' || ch == '\0') {
                    current_state = DONE;
                } else if (!isspace(ch)) {
                    current_state = ERROR;
                }
                break;

            case READING_NUMBER:
                {
                    int number = 0;
                    int valid = 1;
                    
                    while (buffer_index < sizeof(buffer) - 1 && 
                           buffer[buffer_index] != '\0' && 
                           isdigit(buffer[buffer_index])) {
                        int digit = buffer[buffer_index] - '0';
                        if (number > (INT_MAX - digit) / 10) {
                            valid = 0;
                            break;
                        }
                        number = number * 10 + digit;
                        buffer_index++;
                    }
                    
                    if (valid) {
                        if (number_sum <= INT_MAX - number) {
                            number_sum += number;
                        }
                    }
                    
                    if (buffer_index < sizeof(buffer) - 1 && 
                        buffer[buffer_index] != '\0' && 
                        !isspace(buffer[buffer_index])) {
                        current_state = ERROR;
                    } else {
                        current_state = START;
                    }
                }
                break;

            case READING_WORD:
                {
                    int word_length = 0;
                    
                    while (buffer_index < sizeof(buffer) - 1 && 
                           buffer[buffer_index] != '\0' && 
                           isalpha(buffer[buffer_index])) {
                        word_length++;
                        buffer_index++;
                    }
                    
                    if (word_length > 0) {
                        word_count++;
                    }
                    
                    if (buffer_index < sizeof(buffer) - 1 && 
                        buffer[buffer_index] != '\0' && 
                        !isspace(buffer[buffer_index])) {
                        current_state = ERROR;
                    } else {
                        current_state = START;
                    }
                }
                break;

            case ERROR:
                printf("Invalid input format\n");
                return 1;

            case DONE:
                break;
        }

        if (current_state == DONE) {
            break;
        }
        
        if (current_state != READING_NUMBER && current_state != READING_WORD) {
            buffer_index++;
        }
    }

    if (current_state == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
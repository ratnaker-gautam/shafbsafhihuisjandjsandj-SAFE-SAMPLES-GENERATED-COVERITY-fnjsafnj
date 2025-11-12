//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    enum State current_state = INIT;
    char buffer[256];
    int buffer_index = 0;
    int ch;
    int number_sum = 0;
    int word_count = 0;
    int number_value = 0;
    int is_negative = 0;

    printf("Enter text (numbers and words, end with '.'): ");

    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            current_state = ERROR;
            break;
        }

        switch (current_state) {
            case INIT:
                if (ch == '.') {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    number_value = ch - '0';
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    continue;
                } else if (ch == '-') {
                    is_negative = 1;
                    current_state = READING_NUMBER;
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_NUMBER:
                if (isdigit(ch)) {
                    int digit = ch - '0';
                    if (number_value > (INT_MAX - digit) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    number_value = number_value * 10 + digit;
                } else if (isspace(ch) || ch == '.') {
                    if (is_negative) {
                        number_value = -number_value;
                    }
                    number_sum += number_value;
                    number_value = 0;
                    is_negative = 0;
                    if (ch == '.') {
                        current_state = DONE;
                    } else {
                        current_state = INIT;
                    }
                } else {
                    current_state = ERROR;
                }
                break;

            case READING_WORD:
                if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                    }
                } else if (isspace(ch) || ch == '.') {
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        word_count++;
                        buffer_index = 0;
                    }
                    if (ch == '.') {
                        current_state = DONE;
                    } else {
                        current_state = INIT;
                    }
                } else {
                    current_state = ERROR;
                }
                break;

            case ERROR:
                break;

            case DONE:
                break;
        }
    }

    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    if (current_state == READING_NUMBER) {
        if (is_negative) {
            number_value = -number_value;
        }
        number_sum += number_value;
    } else if (current_state == READING_WORD && buffer_index > 0) {
        buffer[buffer_index] = '\0';
        word_count++;
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
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

    printf("Enter text (end with Ctrl+D): ");

    while (current_state != DONE && current_state != ERROR) {
        ch = getchar();
        
        if (ch == EOF) {
            switch (current_state) {
                case START:
                    current_state = DONE;
                    break;
                case READING_NUMBER:
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        buffer_index = 0;
                    }
                    current_state = DONE;
                    break;
                case READING_WORD:
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                    }
                    current_state = DONE;
                    break;
                default:
                    current_state = DONE;
            }
            continue;
        }

        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        current_state = READING_NUMBER;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(ch)) {
                    if (buffer_index < 255) {
                        buffer[buffer_index++] = (char)ch;
                        current_state = READING_WORD;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isspace(ch)) {
                    continue;
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
                    if (buffer_index > 0) {
                        buffer[buffer_index] = '\0';
                        int num = atoi(buffer);
                        number_sum += num;
                        buffer_index = 0;
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
                } else if (isspace(ch)) {
                    if (buffer_index > 0) {
                        word_count++;
                        buffer_index = 0;
                        current_state = START;
                    }
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
    }

    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
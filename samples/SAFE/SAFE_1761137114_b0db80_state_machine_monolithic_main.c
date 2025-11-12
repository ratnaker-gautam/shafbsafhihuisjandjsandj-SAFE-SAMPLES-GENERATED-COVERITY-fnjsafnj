//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

int main(void) {
    enum State current_state = STATE_START;
    char buffer[256];
    int number_sum = 0;
    int word_count = 0;
    int pos = 0;
    int ch;

    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    while (current_state != STATE_END && current_state != STATE_ERROR) {
        ch = buffer[pos];

        switch (current_state) {
            case STATE_START:
                if (ch == '\0') {
                    current_state = STATE_END;
                } else if (isdigit(ch)) {
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(ch)) {
                    current_state = STATE_READING_WORD;
                } else if (isspace(ch)) {
                    pos++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;

            case STATE_READING_NUMBER:
                {
                    int number = 0;
                    int valid = 1;
                    while (isdigit(buffer[pos]) && valid) {
                        int digit = buffer[pos] - '0';
                        if (number > (INT_MAX - digit) / 10) {
                            valid = 0;
                        } else {
                            number = number * 10 + digit;
                        }
                        pos++;
                    }
                    if (valid) {
                        if (number_sum <= INT_MAX - number) {
                            number_sum += number;
                        } else {
                            current_state = STATE_ERROR;
                        }
                    } else {
                        current_state = STATE_ERROR;
                    }
                    current_state = STATE_START;
                }
                break;

            case STATE_READING_WORD:
                {
                    while (isalpha(buffer[pos])) {
                        pos++;
                    }
                    word_count++;
                    current_state = STATE_START;
                }
                break;

            case STATE_ERROR:
                break;

            case STATE_END:
                break;
        }

        if (pos >= 255) {
            current_state = STATE_END;
        }
    }

    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
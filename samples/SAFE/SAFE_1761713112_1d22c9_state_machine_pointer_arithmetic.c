//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n';
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int temp_number = 0;
    int valid_input = 0;

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }

    input[MAX_INPUT] = '\0';
    char *end_ptr = input + strlen(input);

    while (ptr < end_ptr && *ptr != '\0') {
        if (!is_valid_char(*ptr)) {
            return EXIT_FAILURE;
        }

        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    temp_number = *ptr - '0';
                    current_state = READING_NUMBER;
                } else if (isalpha(*ptr)) {
                    word_count++;
                    current_state = READING_WORD;
                } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    current_state = SKIPPING;
                }
                break;

            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if (temp_number <= (INT_MAX - (*ptr - '0')) / 10) {
                        temp_number = temp_number * 10 + (*ptr - '0');
                    } else {
                        return EXIT_FAILURE;
                    }
                } else if (isalpha(*ptr)) {
                    number_sum += temp_number;
                    word_count++;
                    current_state = READING_WORD;
                } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    number_sum += temp_number;
                    current_state = SKIPPING;
                } else {
                    return EXIT_FAILURE;
                }
                break;

            case READING_WORD:
                if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    current_state = SKIPPING;
                } else if (!isalpha(*ptr)) {
                    return EXIT_FAILURE;
                }
                break;

            case SKIPPING:
                if (isdigit(*ptr)) {
                    temp_number = *ptr - '0';
                    current_state = READING_NUMBER;
                } else if (isalpha(*ptr)) {
                    word_count++;
                    current_state = READING_WORD;
                } else if (*ptr != ' ' && *ptr != '\t' && *ptr != '\n') {
                    return EXIT_FAILURE;
                }
                break;

            case FINISHED:
                break;
        }

        if (current_state != FINISHED) {
            ptr++;
        }
    }

    if (current_state == READING_NUMBER) {
        number_sum += temp_number;
    }

    if (word_count > 0 || number_sum > 0) {
        valid_input = 1;
    }

    if (!valid_input) {
        return EXIT_FAILURE;
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return EXIT_SUCCESS;
}
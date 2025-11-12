//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    char *end = input + MAX_INPUT;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int temp_number = 0;
    int valid_input = 0;

    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    while (ptr < end && *ptr != '\0' && current_state != FINISHED) {
        if (!is_valid_char(*ptr)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }

        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    temp_number = *ptr - '0';
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;

            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if (temp_number > (INT_MAX - (*ptr - '0')) / 10) {
                        fprintf(stderr, "Number too large\n");
                        return 1;
                    }
                    temp_number = temp_number * 10 + (*ptr - '0');
                    ptr++;
                } else {
                    if (number_sum > INT_MAX - temp_number) {
                        fprintf(stderr, "Sum overflow\n");
                        return 1;
                    }
                    number_sum += temp_number;
                    temp_number = 0;
                    current_state = SKIPPING;
                }
                break;

            case READING_WORD:
                if (isalpha(*ptr)) {
                    ptr++;
                } else {
                    current_state = SKIPPING;
                }
                break;

            case SKIPPING:
                if (*ptr == '\n' || *ptr == '\0') {
                    current_state = FINISHED;
                } else if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    temp_number = *ptr - '0';
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                }
                ptr++;
                break;

            case FINISHED:
                break;
        }
    }

    if (current_state == READING_NUMBER) {
        if (number_sum > INT_MAX - temp_number) {
            fprintf(stderr, "Sum overflow\n");
            return 1;
        }
        number_sum += temp_number;
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
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
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int temp_number = 0;
    int in_number = 0;

    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    char *end = ptr + strlen(ptr);
    if (end > ptr && *(end - 1) == '\n') {
        *(end - 1) = '\0';
        end--;
    }

    while (ptr < end && current_state != FINISHED) {
        if (!is_valid_char(*ptr)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }

        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    temp_number = *ptr - '0';
                    in_number = 1;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;

            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    if (temp_number > (INT_MAX - (*ptr - '0')) / 10) {
                        fprintf(stderr, "Number overflow detected\n");
                        return 1;
                    }
                    temp_number = temp_number * 10 + (*ptr - '0');
                    ptr++;
                } else {
                    number_sum += temp_number;
                    temp_number = 0;
                    in_number = 0;
                    current_state = START;
                }
                break;

            case READING_WORD:
                if (isalpha((unsigned char)*ptr)) {
                    ptr++;
                } else {
                    current_state = START;
                }
                break;

            case SKIPPING:
                if (!isspace((unsigned char)*ptr)) {
                    current_state = START;
                } else {
                    ptr++;
                }
                break;

            case FINISHED:
                break;
        }

        if (ptr >= end && in_number) {
            number_sum += temp_number;
            in_number = 0;
        }
    }

    if (current_state == READING_NUMBER && in_number) {
        number_sum += temp_number;
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
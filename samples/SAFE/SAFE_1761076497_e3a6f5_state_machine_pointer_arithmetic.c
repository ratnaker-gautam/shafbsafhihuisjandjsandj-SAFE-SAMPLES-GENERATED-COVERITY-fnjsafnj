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
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
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

    char *end_ptr = input + strlen(input);
    if (end_ptr > input && *(end_ptr - 1) == '\n') {
        *(end_ptr - 1) = '\0';
        end_ptr--;
    }

    for (ptr = input; ptr < end_ptr; ptr++) {
        if (!is_valid_char(*ptr)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }

    ptr = input;
    while (ptr < end_ptr && current_state != FINISHED) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    temp_number = *ptr - '0';
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)*ptr)) {
                    word_count++;
                    current_state = READING_WORD;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;

            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    if (temp_number <= (INT_MAX - (*ptr - '0')) / 10) {
                        temp_number = temp_number * 10 + (*ptr - '0');
                    } else {
                        fprintf(stderr, "Number too large\n");
                        return 1;
                    }
                    ptr++;
                } else {
                    number_sum += temp_number;
                    temp_number = 0;
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
                if (!isalnum((unsigned char)*ptr)) {
                    ptr++;
                } else {
                    current_state = START;
                }
                break;

            case FINISHED:
                break;
        }
    }

    if (current_state == READING_NUMBER) {
        number_sum += temp_number;
    }

    if (ptr == end_ptr) {
        current_state = FINISHED;
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
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
    int valid_input = 0;

    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    char *end_ptr = ptr + strlen(ptr);
    while (ptr < end_ptr && *ptr != '\0') {
        if (!is_valid_char(*ptr) && *ptr != '\n') {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        ptr++;
    }

    ptr = input;
    while (current_state != FINISHED && ptr < end_ptr) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    temp_number = *ptr - '0';
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else if (*ptr == '\n' || *ptr == '\0') {
                    current_state = FINISHED;
                }
                break;

            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    if (temp_number <= (INT_MAX - (*ptr - '0')) / 10) {
                        temp_number = temp_number * 10 + (*ptr - '0');
                    } else {
                        fprintf(stderr, "Number too large\n");
                        return 1;
                    }
                } else if (isalpha((unsigned char)*ptr)) {
                    number_sum += temp_number;
                    temp_number = 0;
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)*ptr)) {
                    number_sum += temp_number;
                    temp_number = 0;
                    current_state = SKIPPING;
                } else if (*ptr == '\n' || *ptr == '\0') {
                    number_sum += temp_number;
                    current_state = FINISHED;
                }
                break;

            case READING_WORD:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    temp_number = *ptr - '0';
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else if (*ptr == '\n' || *ptr == '\0') {
                    current_state = FINISHED;
                }
                break;

            case SKIPPING:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    temp_number = *ptr - '0';
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (*ptr == '\n' || *ptr == '\0') {
                    current_state = FINISHED;
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

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
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
    char temp_num[32];
    char *num_ptr;
    int valid_input = 0;

    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    char *end_ptr = ptr + input_len;
    while (ptr < end_ptr && *ptr != '\0') {
        if (!is_valid_char(*ptr)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        ptr++;
    }

    ptr = input;
    while (current_state != FINISHED && ptr < end_ptr) {
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    num_ptr = temp_num;
                    if (num_ptr < temp_num + sizeof(temp_num) - 1) {
                        *num_ptr = *ptr;
                        num_ptr++;
                    }
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
                    if (num_ptr < temp_num + sizeof(temp_num) - 1) {
                        *num_ptr = *ptr;
                        num_ptr++;
                    }
                    ptr++;
                } else {
                    if (num_ptr < temp_num + sizeof(temp_num)) {
                        *num_ptr = '\0';
                    }
                    int num = atoi(temp_num);
                    if (num > 0) {
                        if (number_sum <= INT_MAX - num) {
                            number_sum += num;
                        }
                    }
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
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    num_ptr = temp_num;
                    if (num_ptr < temp_num + sizeof(temp_num) - 1) {
                        *num_ptr = *ptr;
                        num_ptr++;
                    }
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (*ptr == '\0' || *ptr == '\n') {
                    current_state = FINISHED;
                    break;
                }
                ptr++;
                break;

            case FINISHED:
                break;
        }
    }

    if (current_state == READING_NUMBER) {
        if (num_ptr < temp_num + sizeof(temp_num)) {
            *num_ptr = '\0';
        }
        int num = atoi(temp_num);
        if (num > 0) {
            if (number_sum <= INT_MAX - num) {
                number_sum += num;
            }
        }
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
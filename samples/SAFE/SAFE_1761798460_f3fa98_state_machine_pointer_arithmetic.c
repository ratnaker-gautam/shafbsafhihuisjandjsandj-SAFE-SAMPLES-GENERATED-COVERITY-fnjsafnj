//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    char temp_num[32];
    char *num_ptr;
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

    for (char *check = input; check < end_ptr; check++) {
        if (!is_valid_char(*check)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }

    while (ptr < end_ptr && current_state != FINISHED) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    num_ptr = temp_num;
                    *num_ptr = *ptr;
                    num_ptr++;
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
                    if ((num_ptr - temp_num) < 31) {
                        *num_ptr = *ptr;
                        num_ptr++;
                    }
                    ptr++;
                } else {
                    *num_ptr = '\0';
                    number_sum += atoi(temp_num);
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

        if (ptr >= end_ptr && current_state == READING_NUMBER) {
            *num_ptr = '\0';
            number_sum += atoi(temp_num);
            current_state = FINISHED;
        } else if (ptr >= end_ptr) {
            current_state = FINISHED;
        }
    }

    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);

    return 0;
}
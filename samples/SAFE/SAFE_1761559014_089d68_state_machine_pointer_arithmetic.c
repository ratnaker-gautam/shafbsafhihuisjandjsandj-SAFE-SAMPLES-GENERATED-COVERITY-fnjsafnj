//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_WORD,
    READING_NUMBER,
    SKIPPING,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c) || c == '.' || c == ',';
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    State current_state = START;
    int word_count = 0;
    int number_count = 0;
    int valid_input = 1;

    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }

    char *end_ptr = ptr + len;
    while (ptr < end_ptr && valid_input) {
        if (!is_valid_char(*ptr)) {
            valid_input = 0;
            break;
        }

        switch (current_state) {
            case START:
                if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;

            case READING_WORD:
                if (isspace((unsigned char)*ptr)) {
                    current_state = START;
                } else if (!isalpha((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                }
                ptr++;
                break;

            case READING_NUMBER:
                if (isspace((unsigned char)*ptr)) {
                    current_state = START;
                } else if (!isdigit((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                }
                ptr++;
                break;

            case SKIPPING:
                if (isspace((unsigned char)*ptr)) {
                    current_state = START;
                }
                ptr++;
                break;

            case FINISHED:
                break;
        }

        if (ptr >= end_ptr) {
            current_state = FINISHED;
        }
    }

    if (!valid_input) {
        fprintf(stderr, "Invalid characters in input\n");
        return 1;
    }

    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);

    return 0;
}
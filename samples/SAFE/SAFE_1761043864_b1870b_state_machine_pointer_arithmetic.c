//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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

int process_input(char *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT) {
        return -1;
    }

    State current_state = START;
    char *ptr = input;
    char *end = input + len;
    int number_count = 0;
    int word_count = 0;

    while (current_state != FINISHED && ptr < end) {
        if (!is_valid_char(*ptr)) {
            return -1;
        }

        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else {
                    return -1;
                }
                break;

            case READING_NUMBER:
                if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else if (!isdigit((unsigned char)*ptr)) {
                    return -1;
                }
                break;

            case READING_WORD:
                if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else if (!isalpha((unsigned char)*ptr)) {
                    return -1;
                }
                break;

            case SKIPPING:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    number_count++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else if (!isspace((unsigned char)*ptr)) {
                    return -1;
                }
                break;

            case FINISHED:
                break;
        }

        ptr++;
        if (ptr >= end) {
            current_state = FINISHED;
        }
    }

    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    return 0;
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *result = fgets(input, sizeof(input), stdin);
    
    if (result == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }

    if (process_input(input, len) != 0) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }

    return 0;
}
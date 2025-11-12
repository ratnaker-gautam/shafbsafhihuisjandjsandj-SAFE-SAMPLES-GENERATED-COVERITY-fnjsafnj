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

int process_input(const char *input, int *numbers, char *words, int max_count) {
    State current_state = START;
    const char *ptr = input;
    int num_count = 0;
    int word_count = 0;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;

    while (*ptr != '\0' && current_state != FINISHED) {
        if (!is_valid_char(*ptr)) {
            return -1;
        }

        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *ptr;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *ptr;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else {
                    return -1;
                }
                break;

            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    if ((buf_ptr - buffer) < (MAX_INPUT - 1)) {
                        *(buf_ptr++) = *ptr;
                    } else {
                        return -1;
                    }
                } else if (isspace((unsigned char)*ptr)) {
                    *buf_ptr = '\0';
                    if (num_count < max_count) {
                        numbers[num_count] = atoi(buffer);
                        num_count++;
                    }
                    current_state = SKIPPING;
                } else if (isalpha((unsigned char)*ptr)) {
                    return -1;
                } else {
                    current_state = FINISHED;
                }
                break;

            case READING_WORD:
                if (isalpha((unsigned char)*ptr)) {
                    if ((buf_ptr - buffer) < (MAX_INPUT - 1)) {
                        *(buf_ptr++) = *ptr;
                    } else {
                        return -1;
                    }
                } else if (isspace((unsigned char)*ptr)) {
                    *buf_ptr = '\0';
                    if (word_count < max_count) {
                        char *dest = words + (word_count * MAX_INPUT);
                        strncpy(dest, buffer, MAX_INPUT - 1);
                        dest[MAX_INPUT - 1] = '\0';
                        word_count++;
                    }
                    current_state = SKIPPING;
                } else if (isdigit((unsigned char)*ptr)) {
                    return -1;
                } else {
                    current_state = FINISHED;
                }
                break;

            case SKIPPING:
                if (!isspace((unsigned char)*ptr)) {
                    if (isdigit((unsigned char)*ptr)) {
                        current_state = READING_NUMBER;
                        buf_ptr = buffer;
                        *(buf_ptr++) = *ptr;
                    } else if (isalpha((unsigned char)*ptr)) {
                        current_state = READING_WORD;
                        buf_ptr = buffer;
                        *(buf_ptr++) = *ptr;
                    } else {
                        return -1;
                    }
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
        *buf_ptr = '\0';
        if (num_count < max_count) {
            numbers[num_count] = atoi(buffer);
            num_count++;
        }
    } else if (current_state == READING_WORD) {
        *buf_ptr = '\0';
        if (word_count < max_count) {
            char *dest = words + (word_count * MAX_INPUT);
            strncpy(dest, buffer, MAX_INPUT - 1);
            dest[MAX_INPUT - 1] = '\0';
            word_count++;
        }
    }

    return num_count + word_count;
}

int main() {
    char input[MAX_INPUT];
    int numbers[50];
    char words[50 * MAX_INPUT];
    int total_items;

    printf("Enter text with numbers and words: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (len == 0 || len >= sizeof(input)) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }

    total_items = process_input(input, numbers, words, 50);
    if (total_items < 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }

    printf("Processed %d items:\n", total_items);
    
    int num_count = 0;
    int word_count = 0;
    for (int i = 0; i < total_items; i++) {
        if (num_count < 50 && numbers[num_count] != 0) {
            printf("Number: %d\n", numbers[num_count]);
            num_count++;
        } else {
            char *word_ptr = words + (
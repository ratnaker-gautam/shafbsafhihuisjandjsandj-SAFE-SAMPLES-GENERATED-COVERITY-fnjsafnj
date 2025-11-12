//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_DIGITS,
    READING_LETTERS,
    ERROR,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\0';
}

State process_char(char c, int *digit_count, int *letter_count) {
    if (c == '\0' || c == '\n') {
        return FINISHED;
    }
    
    if (isdigit((unsigned char)c)) {
        if (*digit_count < 100) {
            (*digit_count)++;
        }
        return READING_DIGITS;
    }
    
    if (isalpha((unsigned char)c)) {
        if (*letter_count < 100) {
            (*letter_count)++;
        }
        return READING_LETTERS;
    }
    
    if (c == ' ') {
        return START;
    }
    
    return ERROR;
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    State current_state = START;
    int digit_count = 0;
    int letter_count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fputs("Error reading input\n", stderr);
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_INPUT) {
        fputs("Invalid input length\n", stderr);
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(*(ptr + i))) {
            fputs("Invalid character in input\n", stderr);
            return EXIT_FAILURE;
        }
    }
    
    while (current_state != FINISHED && current_state != ERROR && *ptr != '\0') {
        current_state = process_char(*ptr, &digit_count, &letter_count);
        
        if (current_state == ERROR) {
            fputs("Invalid input format\n", stderr);
            return EXIT_FAILURE;
        }
        
        ptr++;
    }
    
    if (current_state == FINISHED) {
        printf("Digits: %d\n", digit_count);
        printf("Letters: %d\n", letter_count);
    } else {
        fputs("Processing incomplete\n", stderr);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
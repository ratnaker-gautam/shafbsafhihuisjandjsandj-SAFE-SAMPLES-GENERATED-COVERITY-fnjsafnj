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
    return isalnum((unsigned char)c) || c == ' ' || c == '\n';
}

State process_char(State current, char c, int *digit_count, int *letter_count) {
    switch (current) {
        case START:
            if (isdigit((unsigned char)c)) {
                (*digit_count)++;
                return READING_DIGITS;
            } else if (isalpha((unsigned char)c)) {
                (*letter_count)++;
                return READING_LETTERS;
            } else if (c == ' ' || c == '\n') {
                return START;
            } else {
                return ERROR;
            }
            
        case READING_DIGITS:
            if (isdigit((unsigned char)c)) {
                (*digit_count)++;
                return READING_DIGITS;
            } else if (isalpha((unsigned char)c)) {
                (*letter_count)++;
                return READING_LETTERS;
            } else if (c == ' ' || c == '\n') {
                return FINISHED;
            } else {
                return ERROR;
            }
            
        case READING_LETTERS:
            if (isalpha((unsigned char)c)) {
                (*letter_count)++;
                return READING_LETTERS;
            } else if (isdigit((unsigned char)c)) {
                (*digit_count)++;
                return READING_DIGITS;
            } else if (c == ' ' || c == '\n') {
                return FINISHED;
            } else {
                return ERROR;
            }
            
        case FINISHED:
            return FINISHED;
            
        case ERROR:
            return ERROR;
    }
    return ERROR;
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    int digit_count = 0;
    int letter_count = 0;
    State current_state = START;
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    char *end_ptr = ptr + len;
    
    while (ptr < end_ptr && *ptr != '\0') {
        if (!is_valid_char(*ptr)) {
            current_state = ERROR;
            break;
        }
        
        current_state = process_char(current_state, *ptr, &digit_count, &letter_count);
        
        if (current_state == ERROR) {
            break;
        }
        
        ptr++;
        
        if (current_state == FINISHED) {
            break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Invalid input: unexpected character\n");
        return EXIT_FAILURE;
    }
    
    printf("Processing completed successfully\n");
    printf("Digits found: %d\n", digit_count);
    printf("Letters found: %d\n", letter_count);
    
    return EXIT_SUCCESS;
}
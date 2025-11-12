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

State process_char(State current, char c, int *digit_count, int *letter_count) {
    switch (current) {
        case START:
            if (isdigit((unsigned char)c)) {
                (*digit_count)++;
                return READING_DIGITS;
            } else if (isalpha((unsigned char)c)) {
                (*letter_count)++;
                return READING_LETTERS;
            } else if (c == '\n' || c == '\0') {
                return FINISHED;
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
            } else if (c == '\n' || c == '\0') {
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
            } else if (c == '\n' || c == '\0') {
                return FINISHED;
            } else {
                return ERROR;
            }
            
        case ERROR:
            return ERROR;
            
        case FINISHED:
            return FINISHED;
    }
    return ERROR;
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr;
    State current_state = START;
    int digit_count = 0;
    int letter_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    ptr = input;
    while (*ptr != '\0') {
        if (!is_valid_char(*ptr)) {
            current_state = ERROR;
            break;
        }
        
        current_state = process_char(current_state, *ptr, &digit_count, &letter_count);
        
        if (current_state == ERROR) {
            break;
        }
        
        ptr++;
    }
    
    if (current_state != ERROR && current_state != FINISHED) {
        current_state = process_char(current_state, '\0', &digit_count, &letter_count);
    }
    
    switch (current_state) {
        case FINISHED:
            printf("Processing completed successfully.\n");
            printf("Digits: %d, Letters: %d\n", digit_count, letter_count);
            break;
            
        case ERROR:
            printf("Error: Invalid input character encountered.\n");
            break;
            
        default:
            printf("Unexpected state reached.\n");
            break;
    }
    
    return (current_state == FINISHED) ? EXIT_SUCCESS : EXIT_FAILURE;
}
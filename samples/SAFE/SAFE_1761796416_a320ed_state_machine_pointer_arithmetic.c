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
                *digit_count = 1;
                return READING_DIGITS;
            } else if (isalpha((unsigned char)c)) {
                *letter_count = 1;
                return READING_LETTERS;
            } else if (c == '\n') {
                return FINISHED;
            }
            return ERROR;
            
        case READING_DIGITS:
            if (isdigit((unsigned char)c)) {
                (*digit_count)++;
                return READING_DIGITS;
            } else if (isalpha((unsigned char)c)) {
                *letter_count = 1;
                return READING_LETTERS;
            } else if (c == '\n') {
                return FINISHED;
            }
            return ERROR;
            
        case READING_LETTERS:
            if (isalpha((unsigned char)c)) {
                (*letter_count)++;
                return READING_LETTERS;
            } else if (isdigit((unsigned char)c)) {
                *digit_count = 1;
                return READING_DIGITS;
            } else if (c == '\n') {
                return FINISHED;
            }
            return ERROR;
            
        case ERROR:
            return ERROR;
            
        case FINISHED:
            return FINISHED;
    }
    return ERROR;
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    int digit_count = 0;
    int letter_count = 0;
    
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
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(*(ptr + i))) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    State current_state = START;
    char *current_char = ptr;
    
    while (current_state != FINISHED && current_state != ERROR) {
        if (current_char >= ptr + len) {
            current_state = FINISHED;
            break;
        }
        
        current_state = process_char(current_state, *current_char, &digit_count, &letter_count);
        current_char++;
    }
    
    if (current_state == ERROR) {
        printf("Invalid input sequence\n");
        return 1;
    }
    
    printf("Digits: %d, Letters: %d\n", digit_count, letter_count);
    
    return 0;
}
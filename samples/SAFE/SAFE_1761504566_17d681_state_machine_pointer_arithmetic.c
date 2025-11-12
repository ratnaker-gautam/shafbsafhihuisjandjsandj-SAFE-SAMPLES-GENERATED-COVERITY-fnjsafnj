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

int is_delimiter(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

int main(void) {
    char input[MAX_INPUT];
    char *ptr = input;
    char *end = input + MAX_INPUT - 1;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int current_number = 0;
    int negative = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT - 1);
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    *(input + strcspn(input, "\n")) = '\0';
    
    while (current_state != FINISHED && ptr < end && *ptr != '\0') {
        switch (current_state) {
            case START:
                if (*ptr == '\0') {
                    current_state = FINISHED;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    current_number = *ptr - '0';
                    negative = 0;
                    ptr++;
                } else if (*ptr == '-') {
                    current_state = READING_NUMBER;
                    current_number = 0;
                    negative = 1;
                    ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                    ptr++;
                } else {
                    current_state = SKIPPING;
                    ptr++;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    int digit = *ptr - '0';
                    if (current_number > (INT_MAX - digit) / 10) {
                        current_number = INT_MAX;
                    } else {
                        current_number = current_number * 10 + digit;
                    }
                    ptr++;
                } else if (is_delimiter(*ptr)) {
                    if (negative) {
                        current_number = -current_number;
                    }
                    number_sum += current_number;
                    current_state = START;
                } else {
                    current_state = SKIPPING;
                    ptr++;
                }
                break;
                
            case READING_WORD:
                if (is_delimiter(*ptr)) {
                    current_state = START;
                }
                ptr++;
                break;
                
            case SKIPPING:
                if (is_delimiter(*ptr)) {
                    current_state = START;
                }
                ptr++;
                break;
                
            case FINISHED:
                break;
        }
    }
    
    if (current_state == READING_NUMBER) {
        if (negative) {
            current_number = -current_number;
        }
        number_sum += current_number;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
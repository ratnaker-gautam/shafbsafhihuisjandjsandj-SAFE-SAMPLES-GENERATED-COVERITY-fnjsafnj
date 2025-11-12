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
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int temp_number = 0;
    int input_valid = 1;
    
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
    
    for (char *p = input; *p != '\0'; p++) {
        if (!is_valid_char(*p)) {
            input_valid = 0;
            break;
        }
    }
    
    if (!input_valid) {
        fprintf(stderr, "Invalid characters in input\n");
        return 1;
    }
    
    while (current_state != FINISHED && ptr < input + len) {
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    temp_number = *ptr - '0';
                    current_state = READING_NUMBER;
                } else if (isalpha(*ptr)) {
                    word_count++;
                    current_state = READING_WORD;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;
                
            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if (temp_number <= (INT_MAX - (*ptr - '0')) / 10) {
                        temp_number = temp_number * 10 + (*ptr - '0');
                    } else {
                        fprintf(stderr, "Number overflow detected\n");
                        return 1;
                    }
                    ptr++;
                } else {
                    number_sum += temp_number;
                    temp_number = 0;
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha(*ptr)) {
                    ptr++;
                } else {
                    current_state = START;
                }
                break;
                
            case SKIPPING:
                if (!isalnum(*ptr)) {
                    ptr++;
                } else {
                    current_state = START;
                }
                break;
                
            default:
                current_state = FINISHED;
                break;
        }
    }
    
    if (current_state == READING_NUMBER) {
        number_sum += temp_number;
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
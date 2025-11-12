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
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n';
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    int temp_number = 0;
    int char_count = 0;
    
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
        printf("No input provided\n");
        return 0;
    }
    
    char *end_ptr = ptr + len;
    
    while (ptr < end_ptr && current_state != FINISHED) {
        if (!is_valid_char(*ptr)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    temp_number = *ptr - '0';
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if (temp_number > (INT_MAX - (*ptr - '0')) / 10) {
                        fprintf(stderr, "Number overflow detected\n");
                        return 1;
                    }
                    temp_number = temp_number * 10 + (*ptr - '0');
                } else if (isalpha(*ptr)) {
                    number_sum += temp_number;
                    temp_number = 0;
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    number_sum += temp_number;
                    temp_number = 0;
                    current_state = SKIPPING;
                }
                break;
                
            case READING_WORD:
                if (!isalpha(*ptr)) {
                    if (isdigit(*ptr)) {
                        current_state = READING_NUMBER;
                        temp_number = *ptr - '0';
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case SKIPPING:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    temp_number = *ptr - '0';
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        char_count++;
        ptr++;
        
        if (ptr >= end_ptr) {
            if (current_state == READING_NUMBER) {
                number_sum += temp_number;
            }
            current_state = FINISHED;
        }
    }
    
    printf("Processed %d characters\n", char_count);
    printf("Found %d words\n", word_count);
    printf("Sum of numbers: %d\n", number_sum);
    
    return 0;
}
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
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n';
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    char *end = input + MAX_INPUT;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    char temp[32];
    char *temp_ptr;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, MAX_INPUT + 1, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    while (ptr < end && *ptr != '\0' && current_state != FINISHED) {
        if (!is_valid_char(*ptr)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    temp_ptr = temp;
                    *temp_ptr = *ptr;
                    temp_ptr++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;
                
            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if (temp_ptr < temp + 31) {
                        *temp_ptr = *ptr;
                        temp_ptr++;
                    }
                    ptr++;
                } else {
                    *temp_ptr = '\0';
                    int num = atoi(temp);
                    if (num > 0) {
                        number_sum += num;
                    }
                    current_state = isalpha(*ptr) ? READING_WORD : SKIPPING;
                    if (isalpha(*ptr)) {
                        word_count++;
                    }
                }
                break;
                
            case READING_WORD:
                if (isalpha(*ptr)) {
                    ptr++;
                } else {
                    current_state = isdigit(*ptr) ? READING_NUMBER : SKIPPING;
                    if (isdigit(*ptr)) {
                        temp_ptr = temp;
                        *temp_ptr = *ptr;
                        temp_ptr++;
                    }
                }
                break;
                
            case SKIPPING:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    temp_ptr = temp;
                    *temp_ptr = *ptr;
                    temp_ptr++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                }
                ptr++;
                break;
                
            case FINISHED:
                break;
        }
        
        if (ptr >= end || *ptr == '\0') {
            if (current_state == READING_NUMBER) {
                *temp_ptr = '\0';
                int num = atoi(temp);
                if (num > 0) {
                    number_sum += num;
                }
            }
            current_state = FINISHED;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
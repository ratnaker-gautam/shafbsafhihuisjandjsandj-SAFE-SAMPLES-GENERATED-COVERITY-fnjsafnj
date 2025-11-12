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
    char input[MAX_INPUT + 1];
    char *ptr = input;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    char temp_num[32];
    char *num_ptr;
    int input_len;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    for (int i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    while (*ptr != '\0' && current_state != FINISHED) {
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    num_ptr = temp_num;
                    *num_ptr = *ptr;
                    num_ptr++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(*ptr)) {
                    if ((num_ptr - temp_num) < 31) {
                        *num_ptr = *ptr;
                        num_ptr++;
                    }
                } else {
                    *num_ptr = '\0';
                    if (num_ptr - temp_num > 0) {
                        long val = strtol(temp_num, NULL, 10);
                        if (val > 0 && val <= 1000000) {
                            number_sum += (int)val;
                        }
                    }
                    
                    if (isalpha(*ptr)) {
                        current_state = READING_WORD;
                        word_count++;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case READING_WORD:
                if (!isalpha(*ptr)) {
                    if (isdigit(*ptr)) {
                        current_state = READING_NUMBER;
                        num_ptr = temp_num;
                        *num_ptr = *ptr;
                        num_ptr++;
                    } else {
                        current_state = SKIPPING;
                    }
                }
                break;
                
            case SKIPPING:
                if (isdigit(*ptr)) {
                    current_state = READING_NUMBER;
                    num_ptr = temp_num;
                    *num_ptr = *ptr;
                    num_ptr++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        ptr++;
        
        if (*ptr == '\0') {
            if (current_state == READING_NUMBER) {
                *num_ptr = '\0';
                if (num_ptr - temp_num > 0) {
                    long val = strtol(temp_num, NULL, 10);
                    if (val > 0 && val <= 1000000) {
                        number_sum += (int)val;
                    }
                }
            }
            current_state = FINISHED;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
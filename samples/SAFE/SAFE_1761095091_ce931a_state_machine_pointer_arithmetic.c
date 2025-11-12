//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    int number_sum = 0;
    int word_count = 0;
    State current_state = START;
    
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
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    while (*ptr != '\0' && current_state != FINISHED) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUMBER:
                if (!isdigit((unsigned char)*ptr)) {
                    current_state = START;
                } else {
                    int num = 0;
                    char *num_start = ptr;
                    while (isdigit((unsigned char)*ptr)) {
                        if (num > (INT_MAX - (*ptr - '0')) / 10) {
                            fprintf(stderr, "Number too large\n");
                            return 1;
                        }
                        num = num * 10 + (*ptr - '0');
                        ptr++;
                    }
                    number_sum += num;
                    ptr--;
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (!isalpha((unsigned char)*ptr)) {
                    word_count++;
                    current_state = START;
                }
                break;
                
            case SKIPPING:
                if (!isspace((unsigned char)*ptr)) {
                    current_state = START;
                    ptr--;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        ptr++;
        
        if (*ptr == '\0' && current_state == READING_WORD) {
            word_count++;
            current_state = FINISHED;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
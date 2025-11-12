//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    char temp_num[32];
    char *num_ptr;
    
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
    
    for (char *p = input; *p != '\0'; p++) {
        if (!is_valid_char(*p)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    while (*ptr != '\0' && current_state != FINISHED) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    num_ptr = temp_num;
                    *num_ptr = *ptr;
                    num_ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    word_count++;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    if ((size_t)(num_ptr - temp_num) < sizeof(temp_num) - 1) {
                        *num_ptr = *ptr;
                        num_ptr++;
                    }
                    ptr++;
                } else {
                    *num_ptr = '\0';
                    int num = atoi(temp_num);
                    if (num > 0) {
                        number_sum += num;
                    }
                    current_state = START;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)*ptr)) {
                    ptr++;
                } else {
                    current_state = START;
                }
                break;
                
            case SKIPPING:
                if (!isspace((unsigned char)*ptr)) {
                    current_state = START;
                } else {
                    ptr++;
                }
                break;
                
            case FINISHED:
                break;
        }
        
        if (*ptr == '\0') {
            current_state = FINISHED;
        }
    }
    
    if (current_state == READING_NUMBER) {
        *num_ptr = '\0';
        int num = atoi(temp_num);
        if (num > 0) {
            number_sum += num;
        }
    }
    
    printf("Sum of numbers: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
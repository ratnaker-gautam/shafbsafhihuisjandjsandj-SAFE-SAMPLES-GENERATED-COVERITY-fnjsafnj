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
    FINISH
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n';
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char *ptr = input;
    State current_state = START;
    int digit_count = 0;
    int letter_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(*(ptr + i))) {
            current_state = ERROR;
            break;
        }
    }
    
    if (current_state == ERROR) {
        printf("Invalid characters detected\n");
        return 1;
    }
    
    while (*ptr != '\0' && current_state != ERROR && current_state != FINISH) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_DIGITS;
                    digit_count++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_LETTERS;
                    letter_count++;
                } else if (*ptr == ' ') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)*ptr)) {
                    digit_count++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_LETTERS;
                    letter_count++;
                } else if (*ptr == ' ') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)*ptr)) {
                    letter_count++;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_DIGITS;
                    digit_count++;
                } else if (*ptr == ' ') {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
        
        if (current_state != ERROR) {
            ptr++;
            if (*ptr == '\0') {
                current_state = FINISH;
            }
        }
    }
    
    if (current_state == ERROR) {
        printf("Invalid input sequence\n");
        return 1;
    }
    
    printf("Analysis complete:\n");
    printf("Digits: %d\n", digit_count);
    printf("Letters: %d\n", letter_count);
    printf("Total characters processed: %zu\n", len);
    
    return 0;
}
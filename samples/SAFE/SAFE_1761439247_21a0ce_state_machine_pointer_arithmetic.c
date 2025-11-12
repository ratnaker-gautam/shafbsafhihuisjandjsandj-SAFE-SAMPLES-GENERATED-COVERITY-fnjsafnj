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

State transition(State current, char input, int *digit_count, int *letter_count) {
    switch (current) {
        case START:
            if (isdigit((unsigned char)input)) {
                (*digit_count)++;
                return READING_DIGITS;
            } else if (isalpha((unsigned char)input)) {
                (*letter_count)++;
                return READING_LETTERS;
            } else if (input == ' ' || input == '\n') {
                return START;
            } else {
                return ERROR;
            }
            
        case READING_DIGITS:
            if (isdigit((unsigned char)input)) {
                (*digit_count)++;
                return READING_DIGITS;
            } else if (isalpha((unsigned char)input)) {
                (*letter_count)++;
                return READING_LETTERS;
            } else if (input == ' ' || input == '\n') {
                return FINISH;
            } else {
                return ERROR;
            }
            
        case READING_LETTERS:
            if (isalpha((unsigned char)input)) {
                (*letter_count)++;
                return READING_LETTERS;
            } else if (isdigit((unsigned char)input)) {
                (*digit_count)++;
                return READING_DIGITS;
            } else if (input == ' ' || input == '\n') {
                return FINISH;
            } else {
                return ERROR;
            }
            
        case ERROR:
            return ERROR;
            
        case FINISH:
            return FINISH;
    }
    return ERROR;
}

int main(void) {
    char input_buffer[MAX_INPUT + 1];
    char *ptr = input_buffer;
    int digit_count = 0;
    int letter_count = 0;
    State current_state = START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    char *end_ptr = ptr + input_len;
    
    while (ptr < end_ptr && current_state != ERROR && current_state != FINISH) {
        if (!is_valid_char(*ptr)) {
            current_state = ERROR;
            break;
        }
        
        current_state = transition(current_state, *ptr, &digit_count, &letter_count);
        ptr++;
    }
    
    if (current_state == ERROR) {
        printf("Invalid input detected\n");
        return 1;
    }
    
    printf("Processed successfully\n");
    printf("Digits: %d\n", digit_count);
    printf("Letters: %d\n", letter_count);
    
    return 0;
}
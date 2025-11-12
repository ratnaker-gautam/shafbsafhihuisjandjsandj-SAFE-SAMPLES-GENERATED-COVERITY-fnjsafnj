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
    DONE
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\0';
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    int digit_count = 0;
    int letter_count = 0;
    
    while (current_state != DONE && current_state != ERROR) {
        char current_char = *ptr;
        
        if (!is_valid_char(current_char)) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)current_char)) {
                    current_state = READING_DIGITS;
                    digit_count++;
                } else if (isalpha((unsigned char)current_char)) {
                    current_state = READING_LETTERS;
                    letter_count++;
                } else if (current_char == ' ' || current_char == '\n') {
                    ptr++;
                    continue;
                } else if (current_char == '\0') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)current_char)) {
                    digit_count++;
                } else if (isalpha((unsigned char)current_char)) {
                    current_state = READING_LETTERS;
                    letter_count++;
                } else if (current_char == ' ' || current_char == '\n') {
                    current_state = START;
                } else if (current_char == '\0') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)current_char)) {
                    letter_count++;
                } else if (isdigit((unsigned char)current_char)) {
                    current_state = READING_DIGITS;
                    digit_count++;
                } else if (current_char == ' ' || current_char == '\n') {
                    current_state = START;
                } else if (current_char == '\0') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
        
        if (current_state != DONE && current_state != ERROR) {
            ptr++;
        }
    }
    
    if (current_state == ERROR) {
        printf("Error: Invalid input character at position %ld\n", (long)(ptr - input));
    } else {
        printf("Processed input successfully.\n");
        printf("Digits found: %d\n", digit_count);
        printf("Letters found: %d\n", letter_count);
    }
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    process_input(input);
    return EXIT_SUCCESS;
}
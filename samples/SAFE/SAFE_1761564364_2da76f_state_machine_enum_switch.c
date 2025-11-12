//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_INTEGER,
    READING_FRACTION,
    READING_EXPONENT,
    READING_EXP_SIGN,
    READING_EXP_DIGITS,
    ERROR,
    SUCCESS
};

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int main(void) {
    char input[256];
    printf("Enter a number to validate: ");
    
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    enum state current = START;
    size_t pos = 0;
    int has_digits = 0;
    
    while (pos < len && current != ERROR && current != SUCCESS) {
        char c = input[pos];
        
        switch (current) {
            case START:
                if (c == '+' || c == '-') {
                    current = READING_INTEGER;
                    pos++;
                } else if (is_digit(c)) {
                    current = READING_INTEGER;
                    has_digits = 1;
                    pos++;
                } else if (c == '.') {
                    current = READING_FRACTION;
                    pos++;
                } else {
                    current = ERROR;
                }
                break;
                
            case READING_INTEGER:
                if (is_digit(c)) {
                    has_digits = 1;
                    pos++;
                } else if (c == '.') {
                    current = READING_FRACTION;
                    pos++;
                } else if (c == 'e' || c == 'E') {
                    current = READING_EXPONENT;
                    pos++;
                } else {
                    current = ERROR;
                }
                break;
                
            case READING_FRACTION:
                if (is_digit(c)) {
                    has_digits = 1;
                    pos++;
                } else if (c == 'e' || c == 'E') {
                    if (has_digits) {
                        current = READING_EXPONENT;
                        pos++;
                    } else {
                        current = ERROR;
                    }
                } else {
                    current = ERROR;
                }
                break;
                
            case READING_EXPONENT:
                if (c == '+' || c == '-') {
                    current = READING_EXP_SIGN;
                    pos++;
                } else if (is_digit(c)) {
                    current = READING_EXP_DIGITS;
                    pos++;
                } else {
                    current = ERROR;
                }
                break;
                
            case READING_EXP_SIGN:
                if (is_digit(c)) {
                    current = READING_EXP_DIGITS;
                    pos++;
                } else {
                    current = ERROR;
                }
                break;
                
            case READING_EXP_DIGITS:
                if (is_digit(c)) {
                    pos++;
                } else {
                    current = ERROR;
                }
                break;
                
            default:
                current = ERROR;
                break;
        }
    }
    
    if (current == ERROR || !has_digits) {
        printf("Invalid number format\n");
        return 1;
    }
    
    if (current == READING_INTEGER || current == READING_FRACTION || current == READING_EXP_DIGITS) {
        printf("Valid number format\n");
        return 0;
    }
    
    printf("Invalid number format\n");
    return 1;
}
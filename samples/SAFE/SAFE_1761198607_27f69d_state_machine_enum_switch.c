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
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    if (len >= sizeof(input) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    enum state current_state = START;
    size_t pos = 0;
    char c;
    
    while (pos < len) {
        c = input[pos];
        
        switch (current_state) {
            case START:
                if (c == '+' || c == '-') {
                    current_state = READING_INTEGER;
                    pos++;
                } else if (is_digit(c)) {
                    current_state = READING_INTEGER;
                    pos++;
                } else if (c == '.') {
                    current_state = READING_FRACTION;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_INTEGER:
                if (is_digit(c)) {
                    pos++;
                } else if (c == '.') {
                    current_state = READING_FRACTION;
                    pos++;
                } else if (c == 'e' || c == 'E') {
                    current_state = READING_EXPONENT;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_FRACTION:
                if (is_digit(c)) {
                    pos++;
                } else if (c == 'e' || c == 'E') {
                    current_state = READING_EXPONENT;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_EXPONENT:
                if (c == '+' || c == '-') {
                    current_state = READING_EXP_SIGN;
                    pos++;
                } else if (is_digit(c)) {
                    current_state = READING_EXP_DIGITS;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_EXP_SIGN:
                if (is_digit(c)) {
                    current_state = READING_EXP_DIGITS;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_EXP_DIGITS:
                if (is_digit(c)) {
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                pos = len;
                break;
                
            case SUCCESS:
                pos = len;
                break;
        }
        
        if (pos == len && current_state != ERROR) {
            if (current_state == READING_INTEGER || 
                current_state == READING_FRACTION || 
                current_state == READING_EXP_DIGITS) {
                current_state = SUCCESS;
            } else {
                current_state = ERROR;
            }
        }
    }
    
    if (current_state == SUCCESS) {
        printf("Valid number format\n");
        return 0;
    } else {
        printf("Invalid number format\n");
        return 1;
    }
}
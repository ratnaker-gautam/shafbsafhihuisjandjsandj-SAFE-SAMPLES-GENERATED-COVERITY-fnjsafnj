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

int main(void) {
    char buffer[256];
    printf("Enter a number to validate: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    enum state current_state = START;
    size_t pos = 0;
    int has_digits = 0;
    int exp_has_digits = 0;
    
    while (pos < len && current_state != ERROR && current_state != SUCCESS) {
        char c = buffer[pos];
        
        switch (current_state) {
            case START:
                if (c == '+' || c == '-') {
                    current_state = READING_INTEGER;
                    pos++;
                } else if (isdigit(c)) {
                    current_state = READING_INTEGER;
                    has_digits = 1;
                    pos++;
                } else if (c == '.') {
                    current_state = READING_FRACTION;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_INTEGER:
                if (isdigit(c)) {
                    has_digits = 1;
                    pos++;
                } else if (c == '.') {
                    current_state = READING_FRACTION;
                    pos++;
                } else if (c == 'e' || c == 'E') {
                    current_state = READING_EXPONENT;
                    pos++;
                } else if (pos == len) {
                    current_state = has_digits ? SUCCESS : ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_FRACTION:
                if (isdigit(c)) {
                    has_digits = 1;
                    pos++;
                } else if (c == 'e' || c == 'E') {
                    if (has_digits) {
                        current_state = READING_EXPONENT;
                        pos++;
                    } else {
                        current_state = ERROR;
                    }
                } else if (pos == len) {
                    current_state = has_digits ? SUCCESS : ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_EXPONENT:
                if (c == '+' || c == '-') {
                    current_state = READING_EXP_SIGN;
                    pos++;
                } else if (isdigit(c)) {
                    current_state = READING_EXP_DIGITS;
                    exp_has_digits = 1;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_EXP_SIGN:
                if (isdigit(c)) {
                    current_state = READING_EXP_DIGITS;
                    exp_has_digits = 1;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_EXP_DIGITS:
                if (isdigit(c)) {
                    exp_has_digits = 1;
                    pos++;
                } else if (pos == len) {
                    current_state = exp_has_digits ? SUCCESS : ERROR;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
            case SUCCESS:
                break;
        }
    }
    
    if (current_state == SUCCESS || (current_state == READING_INTEGER && has_digits) || 
        (current_state == READING_FRACTION && has_digits) || 
        (current_state == READING_EXP_DIGITS && exp_has_digits)) {
        printf("Valid number format\n");
        return 0;
    } else {
        printf("Invalid number format\n");
        return 1;
    }
}
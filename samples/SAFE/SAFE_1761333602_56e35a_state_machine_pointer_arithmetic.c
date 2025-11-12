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
    return isalnum((unsigned char)c) || c == ' ' || c == '\n';
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char output[MAX_INPUT + 1] = {0};
    char *in_ptr = input;
    char *out_ptr = output;
    
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
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    State current_state = START;
    int digit_count = 0;
    int letter_count = 0;
    
    while (*in_ptr != '\0' && current_state != ERROR && current_state != DONE) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*in_ptr)) {
                    current_state = READING_DIGITS;
                    digit_count = 0;
                } else if (isalpha((unsigned char)*in_ptr)) {
                    current_state = READING_LETTERS;
                    letter_count = 0;
                } else if (*in_ptr == ' ') {
                    in_ptr++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)*in_ptr)) {
                    if (digit_count < 10) {
                        *out_ptr = *in_ptr;
                        out_ptr++;
                        digit_count++;
                    }
                    in_ptr++;
                } else if (*in_ptr == ' ' || *in_ptr == '\0') {
                    if (digit_count > 0) {
                        *out_ptr = ' ';
                        out_ptr++;
                    }
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)*in_ptr)) {
                    if (letter_count < 20) {
                        *out_ptr = *in_ptr;
                        out_ptr++;
                        letter_count++;
                    }
                    in_ptr++;
                } else if (*in_ptr == ' ' || *in_ptr == '\0') {
                    if (letter_count > 0) {
                        *out_ptr = ' ';
                        out_ptr++;
                    }
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                break;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (out_ptr > output) {
        if (*(out_ptr - 1) == ' ') {
            *(out_ptr - 1) = '\0';
        } else {
            *out_ptr = '\0';
        }
    } else {
        *out_ptr = '\0';
    }
    
    printf("Processed: %s\n", output);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

int main(void) {
    char buffer[256];
    char output[512] = {0};
    size_t output_pos = 0;
    enum state current_state = START;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len && current_state != ERROR && current_state != DONE; i++) {
        char c = (i < len) ? buffer[i] : '\0';
        
        switch (current_state) {
            case START:
                if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    int digit = c - '0';
                    if (number_sum <= (INT_MAX - digit) / 10) {
                        number_sum = number_sum * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    word_count++;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (c == '\0') {
                    current_state = DONE;
                } else if (!isspace(c)) {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(c)) {
                    int digit = c - '0';
                    if (number_sum <= (INT_MAX - digit) / 10) {
                        number_sum = number_sum * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(c)) {
                    current_state = READING_WORD;
                    word_count++;
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (c == '\0' || isspace(c)) {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(c)) {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (isdigit(c)) {
                    current_state = READING_NUMBER;
                    int digit = c - '0';
                    if (number_sum <= (INT_MAX - digit) / 10) {
                        number_sum = number_sum * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                    if (output_pos < sizeof(output) - 1 && output_pos > 0) {
                        output[output_pos++] = ' ';
                    }
                } else if (c == '\0' || isspace(c)) {
                    current_state = START;
                    if (output_pos < sizeof(output) - 1 && c != '\0') {
                        output[output_pos++] = ' ';
                    }
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
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }
    
    if (output_pos > 0 && output[output_pos-1] == ' ') {
        output[output_pos-1] = '\0';
    } else if (output_pos < sizeof(output)) {
        output[output_pos] = '\0';
    }
    
    printf("Extracted words: %s\n", output);
    printf("Word count: %d\n", word_count);
    printf("Number sum: %d\n", number_sum);
    
    return 0;
}
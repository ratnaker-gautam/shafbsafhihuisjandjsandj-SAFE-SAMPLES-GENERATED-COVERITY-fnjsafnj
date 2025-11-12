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
    int number = 0;
    int number_valid = 0;
    
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
    
    for (size_t i = 0; i <= len && current_state != ERROR && current_state != DONE; i++) {
        char c = (i < len) ? buffer[i] : '\0';
        
        switch (current_state) {
            case START:
                if (c == '\0') {
                    current_state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    number = c - '0';
                    number_valid = 1;
                    current_state = READING_NUMBER;
                } else if (isalpha((unsigned char)c)) {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                    current_state = READING_WORD;
                } else if (c != ' ') {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (c == '\0') {
                    if (number_valid) {
                        char num_str[12];
                        int written = snprintf(num_str, sizeof(num_str), "[%d]", number);
                        if (written > 0 && output_pos + (size_t)written < sizeof(output)) {
                            strcpy(output + output_pos, num_str);
                            output_pos += written;
                        }
                    }
                    current_state = DONE;
                } else if (isdigit((unsigned char)c)) {
                    int digit = c - '0';
                    if (number <= (INT_MAX - digit) / 10) {
                        number = number * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                } else if (c == ' ') {
                    if (number_valid) {
                        char num_str[12];
                        int written = snprintf(num_str, sizeof(num_str), "[%d]", number);
                        if (written > 0 && output_pos + (size_t)written < sizeof(output)) {
                            strcpy(output + output_pos, num_str);
                            output_pos += written;
                        }
                    }
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (c == '\0') {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ' ';
                    }
                    current_state = DONE;
                } else if (isalpha((unsigned char)c)) {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = c;
                    }
                } else if (c == ' ') {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ' ';
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
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    if (output_pos > 0 && output[output_pos-1] == ' ') {
        output[output_pos-1] = '\0';
    } else if (output_pos < sizeof(output)) {
        output[output_pos] = '\0';
    }
    
    printf("Processed: %s\n", output);
    return 0;
}
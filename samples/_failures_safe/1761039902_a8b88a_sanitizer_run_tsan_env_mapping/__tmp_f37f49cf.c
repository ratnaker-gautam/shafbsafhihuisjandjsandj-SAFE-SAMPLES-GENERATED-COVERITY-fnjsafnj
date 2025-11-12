//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    char output[512] = "";
    int buffer_len = 0;
    int output_len = 0;
    enum state current_state = START;
    int ch;
    int number = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    buffer_len = strlen(buffer);
    if (buffer_len > 0 && buffer[buffer_len - 1] == '\n') {
        buffer[buffer_len - 1] = '\0';
        buffer_len--;
    }
    
    if (buffer_len >= sizeof(buffer) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int pos = 0;
    
    while (current_state != DONE && current_state != ERROR && pos <= buffer_len) {
        ch = (pos < buffer_len) ? buffer[pos] : EOF;
        
        switch (current_state) {
            case START:
                if (ch == EOF) {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    number = ch - '0';
                    current_state = READING_NUMBER;
                    pos++;
                } else if (isalpha(ch)) {
                    if (output_len < sizeof(output) - 1) {
                        output[output_len++] = ch;
                    }
                    current_state = READING_WORD;
                    pos++;
                } else if (isspace(ch)) {
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    int digit = ch - '0';
                    if (number > (INT_MAX - digit) / 10) {
                        current_state = ERROR;
                        break;
                    }
                    number = number * 10 + digit;
                    pos++;
                } else if (ch == EOF || isspace(ch)) {
                    char num_str[32];
                    int len = snprintf(num_str, sizeof(num_str), "[%d]", number);
                    if (output_len + len < sizeof(output)) {
                        strcat(output, num_str);
                        output_len += len;
                    }
                    current_state = (ch == EOF) ? DONE : START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalnum(ch)) {
                    if (output_len < sizeof(output) - 1) {
                        output[output_len++] = ch;
                    }
                    pos++;
                } else if (ch == EOF || isspace(ch)) {
                    if (output_len < sizeof(output) - 1) {
                        output[output_len++] = ' ';
                    }
                    current_state = (ch == EOF) ? DONE : START;
                    pos++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (output_len > 0 && output[output_len - 1] == ' ') {
        output[output_len - 1] = '\0';
    }
    
    printf("Processed: %s\n", output);
    return 0;
}
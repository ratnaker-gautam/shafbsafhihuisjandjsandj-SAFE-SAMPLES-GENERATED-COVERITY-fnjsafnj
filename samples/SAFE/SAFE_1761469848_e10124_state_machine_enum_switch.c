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
    char output[512] = "";
    int buffer_len = 0;
    int output_len = 0;
    enum state current_state = START;
    int ch;
    int number = 0;
    int has_number = 0;
    
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
    
    int i = 0;
    while (i <= buffer_len && current_state != ERROR && current_state != DONE) {
        ch = (i < buffer_len) ? buffer[i] : EOF;
        
        switch (current_state) {
            case START:
                if (ch == EOF) {
                    current_state = DONE;
                } else if (isdigit(ch)) {
                    number = ch - '0';
                    has_number = 1;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    if (output_len < sizeof(output) - 1) {
                        output[output_len++] = ch;
                    }
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (ch == EOF || isspace(ch)) {
                    if (has_number && output_len < sizeof(output) - 20) {
                        int written = snprintf(output + output_len, 
                                             sizeof(output) - output_len,
                                             "[NUM:%d]", number);
                        if (written > 0) {
                            output_len += written;
                        }
                    }
                    has_number = 0;
                    number = 0;
                    current_state = (ch == EOF) ? DONE : START;
                } else if (isdigit(ch)) {
                    int digit = ch - '0';
                    if (number <= (INT_MAX - digit) / 10) {
                        number = number * 10 + digit;
                    } else {
                        current_state = ERROR;
                    }
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (ch == EOF || isspace(ch)) {
                    if (output_len < sizeof(output) - 1) {
                        output[output_len++] = ' ';
                    }
                    current_state = (ch == EOF) ? DONE : START;
                } else if (isalnum(ch)) {
                    if (output_len < sizeof(output) - 1) {
                        output[output_len++] = ch;
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
        
        i++;
    }
    
    if (current_state == ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (output_len > 0 && output[output_len - 1] == ' ') {
        output[output_len - 1] = '\0';
    } else if (output_len < sizeof(output)) {
        output[output_len] = '\0';
    }
    
    printf("Processed: %s\n", output);
    return 0;
}
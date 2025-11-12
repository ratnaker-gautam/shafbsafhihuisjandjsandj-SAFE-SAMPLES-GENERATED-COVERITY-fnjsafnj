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
    int ch;
    size_t pos = 0;
    
    printf("Enter text (max 255 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strcspn(buffer, "\n");
    if (len < sizeof(buffer)) {
        buffer[len] = '\0';
    }
    
    while (pos < sizeof(buffer) && buffer[pos] != '\0') {
        ch = buffer[pos];
        
        switch (current_state) {
            case START:
                if (isdigit(ch)) {
                    number = ch - '0';
                    number_valid = 1;
                    current_state = READING_NUMBER;
                } else if (isalpha(ch)) {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ch;
                    }
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_NUMBER:
                if (isdigit(ch)) {
                    if (number <= (INT_MAX - (ch - '0')) / 10) {
                        number = number * 10 + (ch - '0');
                    } else {
                        current_state = ERROR;
                    }
                } else if (isalpha(ch)) {
                    if (number_valid && output_pos < sizeof(output) - 10) {
                        int written = snprintf(output + output_pos, 
                                             sizeof(output) - output_pos,
                                             "[%d]", number);
                        if (written > 0) {
                            output_pos += written;
                        }
                    }
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ch;
                    }
                    number_valid = 0;
                    current_state = READING_WORD;
                } else if (isspace(ch)) {
                    if (number_valid && output_pos < sizeof(output) - 10) {
                        int written = snprintf(output + output_pos,
                                             sizeof(output) - output_pos,
                                             "[%d]", number);
                        if (written > 0) {
                            output_pos += written;
                        }
                    }
                    number_valid = 0;
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_WORD:
                if (isalpha(ch)) {
                    if (output_pos < sizeof(output) - 1) {
                        output[output_pos++] = ch;
                    }
                } else if (isdigit(ch)) {
                    number = ch - '0';
                    number_valid = 1;
                    current_state = READING_NUMBER;
                } else if (isspace(ch)) {
                    current_state = START;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                printf("Invalid input format\n");
                return 1;
                
            case DONE:
                break;
        }
        
        if (current_state == ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
        
        pos++;
    }
    
    if (current_state == READING_NUMBER && number_valid) {
        if (output_pos < sizeof(output) - 10) {
            snprintf(output + output_pos, sizeof(output) - output_pos,
                    "[%d]", number);
        }
    }
    
    if (output_pos < sizeof(output)) {
        output[output_pos] = '\0';
    }
    
    if (output[0] != '\0') {
        printf("Processed: %s\n", output);
    } else {
        printf("No valid content found\n");
    }
    
    return 0;
}
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

void process_input(const char *input, char *output) {
    State current_state = START;
    const char *input_ptr = input;
    char *output_ptr = output;
    
    while (current_state != DONE && current_state != ERROR) {
        char current_char = *input_ptr;
        
        if (!is_valid_char(current_char) && current_char != '\0') {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)current_char)) {
                    current_state = READING_DIGITS;
                    *output_ptr = current_char;
                    output_ptr++;
                } else if (isalpha((unsigned char)current_char)) {
                    current_state = READING_LETTERS;
                    *output_ptr = current_char;
                    output_ptr++;
                } else if (current_char == '\0') {
                    current_state = DONE;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)current_char)) {
                    *output_ptr = current_char;
                    output_ptr++;
                } else if (isalpha((unsigned char)current_char)) {
                    current_state = READING_LETTERS;
                    *output_ptr = ' ';
                    output_ptr++;
                    *output_ptr = current_char;
                    output_ptr++;
                } else if (current_char == '\0') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)current_char)) {
                    *output_ptr = current_char;
                    output_ptr++;
                } else if (isdigit((unsigned char)current_char)) {
                    current_state = READING_DIGITS;
                    *output_ptr = ' ';
                    output_ptr++;
                    *output_ptr = current_char;
                    output_ptr++;
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
            input_ptr++;
            if (output_ptr - output >= MAX_INPUT - 1) {
                current_state = ERROR;
                break;
            }
        }
    }
    
    if (current_state == ERROR) {
        strcpy(output, "ERROR: Invalid input format");
    } else {
        *output_ptr = '\0';
    }
}

int main(void) {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    
    printf("Enter alphanumeric text (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_input(input, output);
    
    printf("Processed: %s\n", output);
    
    return 0;
}
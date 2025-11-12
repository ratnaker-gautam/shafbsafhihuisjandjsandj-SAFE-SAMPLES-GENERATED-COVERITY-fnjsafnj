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
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    char output[MAX_INPUT] = {0};
    char *out_ptr = output;
    
    while (*ptr != '\0' && current_state != ERROR && current_state != FINISHED) {
        if (!is_valid_char(*ptr)) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_DIGITS;
                    *out_ptr = *ptr;
                    out_ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_LETTERS;
                    *out_ptr = *ptr;
                    out_ptr++;
                } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    ptr++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)*ptr)) {
                    *out_ptr = *ptr;
                    out_ptr++;
                } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)*ptr)) {
                    *out_ptr = *ptr;
                    out_ptr++;
                } else if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    current_state = FINISHED;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
            case FINISHED:
                break;
        }
        
        if (current_state != ERROR && current_state != FINISHED) {
            ptr++;
        }
        
        if ((size_t)(out_ptr - output) >= MAX_INPUT - 1) {
            current_state = ERROR;
            break;
        }
    }
    
    *out_ptr = '\0';
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
    } else if (current_state == FINISHED || *output != '\0') {
        printf("Extracted: %s\n", output);
    } else {
        printf("No valid sequence found\n");
    }
}

int main(void) {
    char input[MAX_INPUT] = {0};
    
    printf("Enter text (digits or letters followed by space): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        fprintf(stderr, "Error: Empty input\n");
        return EXIT_FAILURE;
    }
    
    process_input(input);
    
    return EXIT_SUCCESS;
}
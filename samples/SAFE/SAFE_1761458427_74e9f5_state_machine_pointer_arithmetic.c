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
    return isalnum((unsigned char)c) || c == ' ' || c == '\n';
}

int main(void) {
    char input[MAX_INPUT + 1];
    char output[MAX_INPUT + 1];
    char *input_ptr = input;
    char *output_ptr = output;
    State current_state = START;
    int char_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    for (char *p = input; *p != '\0'; p++) {
        if (!is_valid_char(*p)) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    while (*input_ptr != '\0' && char_count < MAX_INPUT) {
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*input_ptr)) {
                    current_state = READING_DIGITS;
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count++;
                } else if (isalpha((unsigned char)*input_ptr)) {
                    current_state = READING_LETTERS;
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count++;
                } else if (*input_ptr == ' ') {
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count++;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)*input_ptr)) {
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count++;
                } else if (*input_ptr == ' ') {
                    current_state = START;
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count++;
                } else if (isalpha((unsigned char)*input_ptr)) {
                    current_state = READING_LETTERS;
                    *output_ptr = ' ';
                    output_ptr++;
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count += 2;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)*input_ptr)) {
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count++;
                } else if (*input_ptr == ' ') {
                    current_state = START;
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count++;
                } else if (isdigit((unsigned char)*input_ptr)) {
                    current_state = READING_DIGITS;
                    *output_ptr = ' ';
                    output_ptr++;
                    *output_ptr = *input_ptr;
                    output_ptr++;
                    char_count += 2;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case ERROR:
                fprintf(stderr, "State machine error at position %ld\n", (long)(input_ptr - input));
                return 1;
                
            case FINISHED:
                break;
        }
        
        if (current_state == ERROR) {
            fprintf(stderr, "Invalid state transition\n");
            return 1;
        }
        
        input_ptr++;
        
        if (*input_ptr == '\0') {
            current_state = FINISHED;
        }
    }
    
    if (char_count >= MAX_INPUT) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    *output_ptr = '\0';
    
    printf("Processed output: %s\n", output);
    
    return 0;
}
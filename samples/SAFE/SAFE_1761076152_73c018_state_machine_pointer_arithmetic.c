//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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

State process_char(char c, char *output, size_t *output_len, size_t max_output) {
    static int digit_count = 0;
    static int letter_count = 0;
    
    if (*output_len >= max_output - 1) {
        return ERROR;
    }
    
    if (c == '\n' || c == '\0') {
        if (digit_count > 0 || letter_count > 0) {
            *(output + *output_len) = '\n';
            (*output_len)++;
            return FINISHED;
        }
        return ERROR;
    }
    
    if (isdigit((unsigned char)c)) {
        if (letter_count > 0) {
            return ERROR;
        }
        *(output + *output_len) = c;
        (*output_len)++;
        digit_count++;
        return READING_DIGITS;
    }
    
    if (isalpha((unsigned char)c)) {
        if (digit_count > 0) {
            return ERROR;
        }
        *(output + *output_len) = c;
        (*output_len)++;
        letter_count++;
        return READING_LETTERS;
    }
    
    if (c == ' ') {
        if (digit_count > 0 || letter_count > 0) {
            return FINISHED;
        }
        return START;
    }
    
    return ERROR;
}

void reset_parser(void) {
    static int digit_count = 0;
    static int letter_count = 0;
    digit_count = 0;
    letter_count = 0;
}

int main(void) {
    char input[MAX_INPUT + 1];
    char output[MAX_INPUT * 2 + 1];
    size_t output_len = 0;
    State current_state = START;
    
    printf("Enter text (digits or letters, separated by spaces): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= sizeof(input)) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(*(input + i))) {
            fprintf(stderr, "Invalid character in input\n");
            return EXIT_FAILURE;
        }
    }
    
    char *current = input;
    while (*current != '\0' && output_len < sizeof(output) - 1) {
        State next_state = process_char(*current, output, &output_len, sizeof(output));
        
        if (next_state == ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return EXIT_FAILURE;
        }
        
        if (next_state == FINISHED) {
            reset_parser();
            current_state = START;
        } else {
            current_state = next_state;
        }
        
        current++;
    }
    
    if (output_len > 0) {
        *(output + output_len) = '\0';
        printf("Processed output:\n%s", output);
    } else {
        printf("No valid sequences found\n");
    }
    
    return EXIT_SUCCESS;
}
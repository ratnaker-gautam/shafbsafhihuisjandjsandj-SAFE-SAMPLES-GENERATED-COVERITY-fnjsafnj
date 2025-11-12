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

State process_char(char c, char *output, size_t *output_pos, size_t max_output) {
    static int digit_count = 0;
    static int letter_count = 0;
    
    if (*output_pos >= max_output - 1) {
        return ERROR;
    }
    
    if (c == '\n') {
        if (digit_count > 0 && letter_count > 0) {
            *(output + *output_pos) = '\0';
            return FINISHED;
        }
        return ERROR;
    }
    
    if (isdigit((unsigned char)c)) {
        *(output + *output_pos) = c;
        (*output_pos)++;
        digit_count++;
        return READING_DIGITS;
    }
    
    if (isalpha((unsigned char)c)) {
        *(output + *output_pos) = c;
        (*output_pos)++;
        letter_count++;
        return READING_LETTERS;
    }
    
    if (c == ' ') {
        *(output + *output_pos) = c;
        (*output_pos)++;
        return START;
    }
    
    return ERROR;
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    char output[MAX_INPUT + 1] = {0};
    size_t output_pos = 0;
    State current_state = START;
    
    printf("Enter text with digits and letters (max %d chars): ", MAX_INPUT);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len > MAX_INPUT) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_char(*(input + i))) {
            fprintf(stderr, "Invalid character in input\n");
            return EXIT_FAILURE;
        }
    }
    
    for (size_t i = 0; i < input_len && current_state != ERROR && current_state != FINISHED; i++) {
        current_state = process_char(*(input + i), output, &output_pos, sizeof(output));
    }
    
    if (current_state == FINISHED) {
        printf("Processed output: %s\n", output);
        return EXIT_SUCCESS;
    } else {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
}
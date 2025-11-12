//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

int is_valid_input(const char *input) {
    if (!input || strlen(input) >= MAX_INPUT) return 0;
    return 1;
}

void process_character(char c, State *state, int *number_sum, int *word_count) {
    switch (*state) {
        case START:
            if (isdigit(c)) {
                *number_sum += (c - '0');
                *state = READING_NUMBER;
            } else if (isalpha(c)) {
                (*word_count)++;
                *state = READING_WORD;
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                *number_sum += (c - '0');
            } else if (isalpha(c)) {
                (*word_count)++;
                *state = READING_WORD;
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_WORD:
            if (!isalpha(c)) {
                if (isdigit(c)) {
                    *number_sum += (c - '0');
                    *state = READING_NUMBER;
                } else {
                    *state = SKIPPING;
                }
            }
            break;
            
        case SKIPPING:
            if (isdigit(c)) {
                *number_sum += (c - '0');
                *state = READING_NUMBER;
            } else if (isalpha(c)) {
                (*word_count)++;
                *state = READING_WORD;
            }
            break;
            
        case FINISHED:
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT + 1];
    char *input_ptr;
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    
    printf("Enter text to analyze: ");
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_ptr = input_buffer;
    
    if (!is_valid_input(input_ptr)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    while (*input_ptr && *input_ptr != '\n') {
        process_character(*input_ptr, &current_state, &number_sum, &word_count);
        input_ptr++;
    }
    
    current_state = FINISHED;
    
    printf("Sum of digits: %d\n", number_sum);
    printf("Words detected: %d\n", word_count);
    
    return 0;
}
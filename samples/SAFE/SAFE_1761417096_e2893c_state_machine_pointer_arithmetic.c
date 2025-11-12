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
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT) return 0;
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

void analyze_string(const char *input, int *number_sum, int *word_count) {
    if (input == NULL || number_sum == NULL || word_count == NULL) return;
    
    *number_sum = 0;
    *word_count = 0;
    State current_state = START;
    
    const char *ptr = input;
    while (*ptr != '\0') {
        process_character(*ptr, &current_state, number_sum, word_count);
        ptr++;
    }
    
    current_state = FINISHED;
}

int main(void) {
    char input_buffer[MAX_INPUT + 1] = {0};
    
    printf("Enter text to analyze: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int total_numbers = 0;
    int total_words = 0;
    
    analyze_string(input_buffer, &total_numbers, &total_words);
    
    printf("Sum of digits: %d\n", total_numbers);
    printf("Word count: %d\n", total_words);
    
    return 0;
}
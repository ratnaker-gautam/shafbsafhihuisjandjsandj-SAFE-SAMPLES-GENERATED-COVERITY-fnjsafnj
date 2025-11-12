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
                *state = READING_NUMBER;
                *number_sum += (c - '0');
            } else if (isalpha(c)) {
                *state = READING_WORD;
                *word_count += 1;
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                *number_sum += (c - '0');
            } else if (isalpha(c)) {
                *state = READING_WORD;
                *word_count += 1;
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_WORD:
            if (!isalpha(c)) {
                if (isdigit(c)) {
                    *state = READING_NUMBER;
                    *number_sum += (c - '0');
                } else {
                    *state = SKIPPING;
                }
            }
            break;
            
        case SKIPPING:
            if (isdigit(c)) {
                *state = READING_NUMBER;
                *number_sum += (c - '0');
            } else if (isalpha(c)) {
                *state = READING_WORD;
                *word_count += 1;
            }
            break;
            
        case FINISHED:
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT + 1];
    char *input_ptr = input_buffer;
    
    printf("Enter text to analyze: ");
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input_buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    State current_state = START;
    int number_sum = 0;
    int word_count = 0;
    char *end_ptr = input_ptr + input_len;
    
    while (input_ptr < end_ptr && current_state != FINISHED) {
        if (*input_ptr == '\0') {
            current_state = FINISHED;
            break;
        }
        
        process_character(*input_ptr, &current_state, &number_sum, &word_count);
        input_ptr++;
    }
    
    printf("Sum of digits: %d\n", number_sum);
    printf("Word count: %d\n", word_count);
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

int is_valid_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 1;
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

void process_character(char c, State *state, int *number, char *word_buf, size_t *word_len) {
    switch (*state) {
        case START:
            if (isdigit(c)) {
                *state = READING_NUMBER;
                *number = c - '0';
            } else if (isalpha(c)) {
                *state = READING_WORD;
                *word_buf = c;
                *word_len = 1;
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                if (*number <= (INT_MAX - (c - '0')) / 10) {
                    *number = *number * 10 + (c - '0');
                } else {
                    *state = SKIPPING;
                    *number = 0;
                }
            } else if (isalpha(c)) {
                *state = READING_WORD;
                *word_buf = c;
                *word_len = 1;
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_WORD:
            if (isalpha(c)) {
                if (*word_len < MAX_INPUT - 1) {
                    *(word_buf + *word_len) = c;
                    (*word_len)++;
                }
            } else if (isdigit(c)) {
                *state = READING_NUMBER;
                *number = c - '0';
            } else {
                *state = SKIPPING;
            }
            break;
            
        case SKIPPING:
            if (isdigit(c)) {
                *state = READING_NUMBER;
                *number = c - '0';
            } else if (isalpha(c)) {
                *state = READING_WORD;
                *word_buf = c;
                *word_len = 1;
            }
            break;
            
        case FINISHED:
            break;
    }
}

int main(void) {
    char input[MAX_INPUT];
    char *input_ptr = input;
    
    printf("Enter text: ");
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strnlen(input, MAX_INPUT);
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    State current_state = START;
    int current_number = 0;
    char current_word[MAX_INPUT];
    size_t word_length = 0;
    
    char *end_ptr = input_ptr + input_len;
    
    while (input_ptr < end_ptr && *input_ptr != '\0' && current_state != FINISHED) {
        char c = *input_ptr;
        
        process_character(c, &current_state, &current_number, current_word, &word_length);
        
        if (current_state == READING_WORD && (input_ptr + 1 >= end_ptr || !isalpha(*(input_ptr + 1)))) {
            if (word_length > 0 && word_length < MAX_INPUT) {
                *(current_word + word_length) = '\0';
                printf("Found word: %s\n", current_word);
            }
            current_state = SKIPPING;
            word_length = 0;
        }
        
        if (current_state == READING_NUMBER && (input_ptr + 1 >= end_ptr || !isdigit(*(input_ptr + 1)))) {
            printf("Found number: %d\n", current_number);
            current_state = SKIPPING;
            current_number = 0;
        }
        
        input_ptr++;
    }
    
    return 0;
}
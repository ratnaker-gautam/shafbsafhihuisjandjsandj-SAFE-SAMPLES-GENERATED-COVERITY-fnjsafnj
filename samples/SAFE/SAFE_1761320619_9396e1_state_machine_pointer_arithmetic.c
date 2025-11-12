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

int is_valid_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(*(input + i))) return 0;
    }
    return 1;
}

void process_character(char c, State *state, int *number, char *word, size_t *word_len) {
    switch (*state) {
        case START:
            if (isdigit(c)) {
                *state = READING_NUMBER;
                *number = c - '0';
            } else if (isalpha(c)) {
                *state = READING_WORD;
                *word_len = 0;
                *(word + *word_len) = c;
                (*word_len)++;
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                *number = *number * 10 + (c - '0');
            } else {
                printf("Found number: %d\n", *number);
                *number = 0;
                if (isalpha(c)) {
                    *state = READING_WORD;
                    *word_len = 0;
                    *(word + *word_len) = c;
                    (*word_len)++;
                } else {
                    *state = SKIPPING;
                }
            }
            break;
            
        case READING_WORD:
            if (isalpha(c)) {
                if (*word_len < MAX_INPUT - 1) {
                    *(word + *word_len) = c;
                    (*word_len)++;
                }
            } else {
                *(word + *word_len) = '\0';
                printf("Found word: %s\n", word);
                *word_len = 0;
                if (isdigit(c)) {
                    *state = READING_NUMBER;
                    *number = c - '0';
                } else {
                    *state = SKIPPING;
                }
            }
            break;
            
        case SKIPPING:
            if (isdigit(c)) {
                *state = READING_NUMBER;
                *number = c - '0';
            } else if (isalpha(c)) {
                *state = READING_WORD;
                *word_len = 0;
                *(word + *word_len) = c;
                (*word_len)++;
            }
            break;
            
        case FINISHED:
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT];
    char *input_ptr = input_buffer;
    size_t input_len;
    
    printf("Enter text (max %d characters): ", MAX_INPUT - 1);
    
    if (fgets(input_ptr, MAX_INPUT, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input_ptr);
    if (input_len > 0 && *(input_ptr + input_len - 1) == '\n') {
        *(input_ptr + input_len - 1) = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input_ptr, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    State current_state = START;
    int current_number = 0;
    char current_word[MAX_INPUT];
    size_t current_word_len = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = *(input_ptr + i);
        process_character(current_char, &current_state, &current_number, current_word, &current_word_len);
    }
    
    if (current_state == READING_NUMBER) {
        printf("Found number: %d\n", current_number);
    } else if (current_state == READING_WORD && current_word_len > 0) {
        *(current_word + current_word_len) = '\0';
        printf("Found word: %s\n", current_word);
    }
    
    printf("Processing complete.\n");
    return 0;
}
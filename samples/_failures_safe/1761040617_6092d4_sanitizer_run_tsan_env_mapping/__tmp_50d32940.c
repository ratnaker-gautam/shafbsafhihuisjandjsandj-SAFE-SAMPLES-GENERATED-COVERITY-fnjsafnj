//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    if (!input || *input == '\0') return 0;
    size_t len = strlen(input);
    return len > 0 && len < MAX_INPUT;
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
                if (*number < 0) *number = 0;
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
    char input[MAX_INPUT];
    char *ptr;
    
    printf("Enter text to process: ");
    if (!fgets(input, sizeof(input), stdin)) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    State current_state = START;
    int current_number = 0;
    char current_word[MAX_INPUT];
    size_t word_length = 0;
    
    ptr = input;
    while (*ptr != '\0' && current_state != FINISHED) {
        process_character(*ptr, &current_state, &current_number, current_word, &word_length);
        ptr++;
    }
    
    if (current_state == READING_NUMBER) {
        printf("Found number: %d\n", current_number);
    } else if (current_state == READING_WORD) {
        if (word_length < MAX_INPUT) {
            *(current_word + word_length) = '\0';
            printf("Found word: %s\n", current_word);
        }
    }
    
    printf("Processing complete.\n");
    return 0;
}
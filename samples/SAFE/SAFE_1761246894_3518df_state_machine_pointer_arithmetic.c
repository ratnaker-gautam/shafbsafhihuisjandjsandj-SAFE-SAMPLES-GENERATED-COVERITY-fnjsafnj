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
                *state = FINISHED;
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
                *state = FINISHED;
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
    char *input_ptr = input;
    
    printf("Enter text (max %d chars): ", MAX_INPUT - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && *(input_ptr + input_len - 1) == '\n') {
        *(input_ptr + input_len - 1) = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    State current_state = START;
    int found_number = 0;
    char found_word[MAX_INPUT];
    size_t word_length = 0;
    
    char *current_char = input_ptr;
    while (*current_char != '\0' && current_state != FINISHED) {
        process_character(*current_char, &current_state, &found_number, found_word, &word_length);
        current_char++;
    }
    
    if (current_state == READING_WORD && word_length > 0) {
        *(found_word + word_length) = '\0';
        current_state = FINISHED;
    }
    
    printf("Processing complete. State: ");
    switch (current_state) {
        case START:
            printf("No valid data found\n");
            break;
        case READING_NUMBER:
            printf("Found number: %d\n", found_number);
            break;
        case READING_WORD:
            printf("Found word: %s\n", found_word);
            break;
        case SKIPPING:
            printf("Skipping characters\n");
            break;
        case FINISHED:
            if (word_length > 0) {
                printf("Found word: %s\n", found_word);
            } else if (found_number > 0) {
                printf("Found number: %d\n", found_number);
            } else {
                printf("No valid data extracted\n");
            }
            break;
    }
    
    return 0;
}
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
                *number = (*number * 10) + (c - '0');
            } else {
                *state = FINISHED;
            }
            break;
            
        case READING_WORD:
            if (isalpha(c)) {
                if (*word_len < MAX_INPUT - 1) {
                    *(word_buf + *word_len) = c;
                    (*word_len)++;
                }
            } else {
                *(word_buf + *word_len) = '\0';
                *state = FINISHED;
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
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && *(input_ptr + input_len - 1) == '\n') {
        *(input_ptr + input_len - 1) = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    State current_state = START;
    int number = 0;
    char word[MAX_INPUT];
    size_t word_len = 0;
    
    char *ptr = input_ptr;
    while (*ptr != '\0' && current_state != FINISHED) {
        process_character(*ptr, &current_state, &number, word, &word_len);
        ptr++;
    }
    
    if (current_state == READING_WORD && word_len > 0) {
        *(word + word_len) = '\0';
        current_state = FINISHED;
    }
    
    switch (current_state) {
        case READING_NUMBER:
            printf("Found number: %d\n", number);
            break;
        case FINISHED:
            if (word_len > 0) {
                printf("Found word: %s\n", word);
            } else {
                printf("Found number: %d\n", number);
            }
            break;
        default:
            printf("No valid token found\n");
            break;
    }
    
    return 0;
}
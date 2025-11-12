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
    if (input == NULL || len == 0 || len >= MAX_INPUT) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') break;
        if (!isprint((unsigned char)input[i])) {
            return 0;
        }
    }
    return 1;
}

void process_character(char c, State *state, int *number, char *word, size_t *word_len) {
    switch (*state) {
        case START:
            if (isdigit((unsigned char)c)) {
                *state = READING_NUMBER;
                *number = c - '0';
            } else if (isalpha((unsigned char)c)) {
                *state = READING_WORD;
                *word_len = 0;
                if (*word_len < MAX_INPUT - 1) {
                    *(word + *word_len) = c;
                    (*word_len)++;
                }
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (*number <= (INT_MAX - (c - '0')) / 10) {
                    *number = *number * 10 + (c - '0');
                }
            } else if (isalpha((unsigned char)c)) {
                *state = READING_WORD;
                *word_len = 0;
                if (*word_len < MAX_INPUT - 1) {
                    *(word + *word_len) = c;
                    (*word_len)++;
                }
            } else {
                *state = SKIPPING;
            }
            break;
            
        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (*word_len < MAX_INPUT - 1) {
                    *(word + *word_len) = c;
                    (*word_len)++;
                }
            } else if (isdigit((unsigned char)c)) {
                *state = READING_NUMBER;
                *number = c - '0';
            } else {
                *state = SKIPPING;
            }
            break;
            
        case SKIPPING:
            if (isdigit((unsigned char)c)) {
                *state = READING_NUMBER;
                *number = c - '0';
            } else if (isalpha((unsigned char)c)) {
                *state = READING_WORD;
                *word_len = 0;
                if (*word_len < MAX_INPUT - 1) {
                    *(word + *word_len) = c;
                    (*word_len)++;
                }
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
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    State current_state = START;
    int current_number = 0;
    char current_word[MAX_INPUT];
    size_t current_word_len = 0;
    
    char *end_ptr = input_ptr + input_len;
    
    while (input_ptr < end_ptr && *input_ptr != '\0') {
        process_character(*input_ptr, &current_state, &current_number, current_word, &current_word_len);
        
        if (current_state == READING_NUMBER && (input_ptr + 1 >= end_ptr || !isdigit((unsigned char)*(input_ptr + 1)))) {
            printf("Found number: %d\n", current_number);
            current_state = SKIPPING;
        }
        
        if (current_state == READING_WORD && (input_ptr + 1 >= end_ptr || !isalpha((unsigned char)*(input_ptr + 1)))) {
            if (current_word_len < MAX_INPUT) {
                *(current_word + current_word_len) = '\0';
                printf("Found word: %s\n", current_word);
            }
            current_state = SKIPPING;
        }
        
        input_ptr++;
    }
    
    current_state = FINISHED;
    printf("Processing complete\n");
    
    return EXIT_SUCCESS;
}
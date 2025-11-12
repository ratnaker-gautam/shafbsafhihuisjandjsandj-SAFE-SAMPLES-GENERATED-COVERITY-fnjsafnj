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
                } else {
                    *state = SKIPPING;
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
    char output[MAX_INPUT * 2];
    char *output_ptr = output;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strnlen(input, sizeof(input));
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    State current_state = START;
    int current_number = 0;
    char current_word[MAX_INPUT];
    size_t current_word_len = 0;
    
    for (char *ptr = input; ptr < input + input_len && *ptr != '\0'; ptr++) {
        process_character(*ptr, &current_state, &current_number, current_word, &current_word_len);
        
        if (current_state == READING_NUMBER && (ptr + 1 >= input + input_len || !isdigit(*(ptr + 1)))) {
            int written = snprintf(output_ptr, output + sizeof(output) - output_ptr, "[NUM:%d] ", current_number);
            if (written > 0 && written < (int)(output + sizeof(output) - output_ptr)) {
                output_ptr += written;
            }
            current_state = START;
            current_number = 0;
        }
        
        if (current_state == READING_WORD && (ptr + 1 >= input + input_len || !isalpha(*(ptr + 1)))) {
            if (current_word_len < sizeof(current_word)) {
                *(current_word + current_word_len) = '\0';
                int written = snprintf(output_ptr, output + sizeof(output) - output_ptr, "[WORD:%s] ", current_word);
                if (written > 0 && written < (int)(output + sizeof(output) - output_ptr)) {
                    output_ptr += written;
                }
            }
            current_state = START;
            current_word_len = 0;
        }
    }
    
    if (output_ptr > output) {
        *(output_ptr - 1) = '\0';
    } else {
        *output_ptr = '\0';
    }
    
    printf("Processed: %s\n", output);
    return 0;
}
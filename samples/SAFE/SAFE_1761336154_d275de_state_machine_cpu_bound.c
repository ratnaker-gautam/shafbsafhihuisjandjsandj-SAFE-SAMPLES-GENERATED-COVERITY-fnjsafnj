//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef struct {
    parser_state_t from;
    parser_state_t to;
    int (*condition)(int);
} transition_t;

int is_digit_char(int c) {
    return isdigit(c);
}

int is_alpha_char(int c) {
    return isalpha(c);
}

int is_space_char(int c) {
    return isspace(c);
}

int is_null_char(int c) {
    return c == '\0';
}

int always_true(int c) {
    return 1;
}

transition_t transitions[] = {
    {STATE_START, STATE_READING_NUMBER, is_digit_char},
    {STATE_START, STATE_READING_WORD, is_alpha_char},
    {STATE_START, STATE_END, is_null_char},
    {STATE_START, STATE_ERROR, always_true},
    {STATE_READING_NUMBER, STATE_READING_NUMBER, is_digit_char},
    {STATE_READING_NUMBER, STATE_START, is_space_char},
    {STATE_READING_NUMBER, STATE_END, is_null_char},
    {STATE_READING_NUMBER, STATE_ERROR, always_true},
    {STATE_READING_WORD, STATE_READING_WORD, is_alpha_char},
    {STATE_READING_WORD, STATE_START, is_space_char},
    {STATE_READING_WORD, STATE_END, is_null_char},
    {STATE_READING_WORD, STATE_ERROR, always_true},
    {STATE_ERROR, STATE_END, is_null_char},
    {STATE_ERROR, STATE_ERROR, always_true}
};

#define NUM_TRANSITIONS (sizeof(transitions)/sizeof(transitions[0]))

void process_input(const char* input) {
    parser_state_t current_state = STATE_START;
    const char* ptr = input;
    int numbers_found = 0;
    int words_found = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        int found_transition = 0;
        
        for (size_t i = 0; i < NUM_TRANSITIONS; i++) {
            if (transitions[i].from == current_state && transitions[i].condition(*ptr)) {
                if (current_state == STATE_READING_NUMBER && transitions[i].to != STATE_READING_NUMBER) {
                    numbers_found++;
                }
                if (current_state == STATE_READING_WORD && transitions[i].to != STATE_READING_WORD) {
                    words_found++;
                }
                
                current_state = transitions[i].to;
                found_transition = 1;
                
                if (current_state != STATE_READING_NUMBER && current_state != STATE_READING_WORD) {
                    ptr++;
                }
                break;
            }
        }
        
        if (!found_transition) {
            current_state = STATE_ERROR;
        }
        
        if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
            ptr++;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
    } else {
        printf("Parsing complete: %d numbers, %d words\n", numbers_found, words_found);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT_LEN && input[MAX_INPUT_LEN - 1] != '\0' && input[MAX_INPUT_LEN - 1] != '\n') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_input(input);
    
    return 0;
}
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

int is_end_char(int c) {
    return c == '\0';
}

int always_true(int c) {
    return 1;
}

transition_t transitions[] = {
    {STATE_START, STATE_READING_NUMBER, is_digit_char},
    {STATE_START, STATE_READING_WORD, is_alpha_char},
    {STATE_START, STATE_END, is_end_char},
    {STATE_START, STATE_ERROR, always_true},
    
    {STATE_READING_NUMBER, STATE_READING_NUMBER, is_digit_char},
    {STATE_READING_NUMBER, STATE_START, is_space_char},
    {STATE_READING_NUMBER, STATE_END, is_end_char},
    {STATE_READING_NUMBER, STATE_ERROR, always_true},
    
    {STATE_READING_WORD, STATE_READING_WORD, is_alpha_char},
    {STATE_READING_WORD, STATE_START, is_space_char},
    {STATE_READING_WORD, STATE_END, is_end_char},
    {STATE_READING_WORD, STATE_ERROR, always_true},
    
    {STATE_ERROR, STATE_END, is_end_char},
    {STATE_ERROR, STATE_ERROR, always_true}
};

#define NUM_TRANSITIONS (sizeof(transitions)/sizeof(transitions[0]))

parser_state_t process_input(const char* input) {
    parser_state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        int found_transition = 0;
        int c = input[i];
        
        for (size_t j = 0; j < NUM_TRANSITIONS; j++) {
            if (transitions[j].from == current_state) {
                if (transitions[j].condition(c)) {
                    current_state = transitions[j].to;
                    found_transition = 1;
                    break;
                }
            }
        }
        
        if (!found_transition) {
            current_state = STATE_ERROR;
        }
        
        if (current_state == STATE_END || current_state == STATE_ERROR) {
            break;
        }
    }
    
    return current_state;
}

void process_cpu_intensive() {
    volatile uint64_t counter = 0;
    for (volatile uint64_t i = 0; i < 1000000; i++) {
        counter += i * i;
        counter = counter % 1000000;
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    printf("Processing input...\n");
    
    process_cpu_intensive();
    
    parser_state_t result = process_input(input);
    
    printf("Parser result: ");
    switch (result) {
        case STATE_END:
            printf("VALID - Input successfully parsed\n");
            break;
        case STATE_ERROR:
            printf("INVALID - Input contains errors\n");
            break;
        default:
            printf("UNEXPECTED - Parser ended in unexpected state\n");
            break;
    }
    
    return 0;
}
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
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t from_state;
    char trigger;
    parser_state_t to_state;
} transition_t;

static transition_t transitions[] = {
    {STATE_START, ' ', STATE_START},
    {STATE_START, '\t', STATE_START},
    {STATE_START, '\n', STATE_START},
    {STATE_START, '0', STATE_READING_NUMBER},
    {STATE_START, '1', STATE_READING_NUMBER},
    {STATE_START, '2', STATE_READING_NUMBER},
    {STATE_START, '3', STATE_READING_NUMBER},
    {STATE_START, '4', STATE_READING_NUMBER},
    {STATE_START, '5', STATE_READING_NUMBER},
    {STATE_START, '6', STATE_READING_NUMBER},
    {STATE_START, '7', STATE_READING_NUMBER},
    {STATE_START, '8', STATE_READING_NUMBER},
    {STATE_START, '9', STATE_READING_NUMBER},
    {STATE_START, 'a', STATE_READING_WORD},
    {STATE_START, 'b', STATE_READING_WORD},
    {STATE_START, 'c', STATE_READING_WORD},
    {STATE_START, 'A', STATE_READING_WORD},
    {STATE_START, 'B', STATE_READING_WORD},
    {STATE_START, 'C', STATE_READING_WORD},
    {STATE_READING_NUMBER, '0', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '1', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '2', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '3', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '4', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '5', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '6', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '7', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '8', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '9', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, ' ', STATE_START},
    {STATE_READING_NUMBER, '\t', STATE_START},
    {STATE_READING_NUMBER, '\n', STATE_START},
    {STATE_READING_WORD, 'a', STATE_READING_WORD},
    {STATE_READING_WORD, 'b', STATE_READING_WORD},
    {STATE_READING_WORD, 'c', STATE_READING_WORD},
    {STATE_READING_WORD, 'A', STATE_READING_WORD},
    {STATE_READING_WORD, 'B', STATE_READING_WORD},
    {STATE_READING_WORD, 'C', STATE_READING_WORD},
    {STATE_READING_WORD, ' ', STATE_START},
    {STATE_READING_WORD, '\t', STATE_START},
    {STATE_READING_WORD, '\n', STATE_START}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

parser_state_t find_transition(parser_state_t current_state, char input_char) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == current_state && transitions[i].trigger == input_char) {
            return transitions[i].to_state;
        }
    }
    return STATE_ERROR;
}

void process_input(const char* input) {
    parser_state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    int number_count = 0;
    int word_count = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        parser_state_t next_state = find_transition(current_state, c);
        
        if (next_state == STATE_ERROR) {
            printf("Invalid input at position %zu: '%c'\n", i, c);
            return;
        }
        
        if (current_state == STATE_READING_NUMBER && (next_state == STATE_START || next_state == STATE_END)) {
            number_count++;
        }
        
        if (current_state == STATE_READING_WORD && (next_state == STATE_START || next_state == STATE_END)) {
            word_count++;
        }
        
        current_state = next_state;
    }
    
    if (current_state == STATE_READING_NUMBER) {
        number_count++;
    }
    
    if (current_state == STATE_READING_WORD) {
        word_count++;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1
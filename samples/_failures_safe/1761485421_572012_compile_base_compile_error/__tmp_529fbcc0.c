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
    STATE_READING_SYMBOL,
    STATE_ERROR,
    STATE_END
} parser_state_t;

typedef struct {
    parser_state_t from_state;
    char trigger_char;
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
    {STATE_START, 'z', STATE_READING_WORD},
    {STATE_START, 'A', STATE_READING_WORD},
    {STATE_START, 'Z', STATE_READING_WORD},
    {STATE_START, '+', STATE_READING_SYMBOL},
    {STATE_START, '-', STATE_READING_SYMBOL},
    {STATE_START, '*', STATE_READING_SYMBOL},
    {STATE_START, '/', STATE_READING_SYMBOL},
    {STATE_READING_NUMBER, '0', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, '9', STATE_READING_NUMBER},
    {STATE_READING_NUMBER, ' ', STATE_START},
    {STATE_READING_NUMBER, '\t', STATE_START},
    {STATE_READING_NUMBER, '\n', STATE_START},
    {STATE_READING_WORD, 'a', STATE_READING_WORD},
    {STATE_READING_WORD, 'z', STATE_READING_WORD},
    {STATE_READING_WORD, 'A', STATE_READING_WORD},
    {STATE_READING_WORD, 'Z', STATE_READING_WORD},
    {STATE_READING_WORD, '0', STATE_READING_WORD},
    {STATE_READING_WORD, '9', STATE_READING_WORD},
    {STATE_READING_WORD, ' ', STATE_START},
    {STATE_READING_WORD, '\t', STATE_START},
    {STATE_READING_WORD, '\n', STATE_START},
    {STATE_READING_SYMBOL, ' ', STATE_START},
    {STATE_READING_SYMBOL, '\t', STATE_START},
    {STATE_READING_SYMBOL, '\n', STATE_START}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static parser_state_t find_next_state(parser_state_t current_state, char input_char) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == current_state) {
            if (transitions[i].trigger_char == input_char) {
                return transitions[i].to_state;
            }
            if (transitions[i].trigger_char == '0' && isdigit(input_char)) {
                return transitions[i].to_state;
            }
            if (transitions[i].trigger_char == '9' && isdigit(input_char)) {
                return transitions[i].to_state;
            }
            if (transitions[i].trigger_char == 'a' && isalpha(input_char)) {
                return transitions[i].to_state;
            }
            if (transitions[i].trigger_char == 'z' && isalpha(input_char)) {
                return transitions[i].to_state;
            }
            if (transitions[i].trigger_char == 'A' && isalpha(input_char)) {
                return transitions[i].to_state;
            }
            if (transitions[i].trigger_char == 'Z' && isalpha(input_char)) {
                return transitions[i].to_state;
            }
        }
    }
    return STATE_ERROR;
}

static void process_state_change(parser_state_t old_state, parser_state_t new_state, char current_char) {
    if (old_state != new_state) {
        if (old_state == STATE_READING_NUMBER) {
            printf("Found number\n");
        } else if (old_state == STATE_READING_WORD) {
            printf("Found word\n");
        } else if (old_state == STATE_READING_SYMBOL) {
            printf("Found symbol\n");
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    printf("Enter text to parse: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL)
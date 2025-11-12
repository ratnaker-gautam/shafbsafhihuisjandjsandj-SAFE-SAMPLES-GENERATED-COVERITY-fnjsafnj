//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    char input_char;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_len;
    size_t position;
    int result;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, ' ', STATE_IDLE},
    {STATE_IDLE, '\n', STATE_IDLE},
    {STATE_IDLE, '\t', STATE_IDLE},
    {STATE_IDLE, '0', STATE_READING},
    {STATE_IDLE, '1', STATE_READING},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, ' ', STATE_PROCESSING},
    {STATE_READING, '\n', STATE_PROCESSING},
    {STATE_READING, '\t', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_PROCESSING},
    {STATE_PROCESSING, ' ', STATE_PROCESSING},
    {STATE_PROCESSING, '\n', STATE_PROCESSING},
    {STATE_PROCESSING, '\t', STATE_PROCESSING},
    {STATE_PROCESSING, '0', STATE_READING},
    {STATE_PROCESSING, '1', STATE_READING},
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_OUTPUT, '\0', STATE_DONE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void init_state_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_len = 0;
    sm->position = 0;
    sm->result = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

int process_binary_number(const char *str, size_t len) {
    if (len == 0 || len > 31) return -1;
    
    int value = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1') return -1;
        if (value > (INT_MAX >> 1)) return -1;
        value = (value << 1) | (str[i] - '0');
    }
    return value;
}

int run_state_machine(StateMachine *sm) {
    char current_char;
    size_t number_start = 0;
    size_t number_len = 0;
    
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        if (sm->position >= sm->input_len) {
            current_char = '\0';
        } else {
            current_char = sm->input_buffer[sm->position];
        }
        
        State next_state = get_next_state(sm->current_state, current_char);
        
        if (next_state == STATE_ERROR) {
            return -1;
        }
        
        if (sm->current_state == STATE_READING && next_state == STATE_PROCESSING) {
            number_len = sm->position - number_start;
            if (number_len > 0) {
                int num = process_binary_number(&sm->input_buffer[number_start], number_len);
                if (num >= 0) {
                    if (sm->result <= INT_MAX - num) {
                        sm->result += num;
                    } else {
                        return -1;
                    }
                } else {
                    return -1;
                }
            }
        }
        
        if (sm->current_state == STATE_IDLE && next_state == STATE_READING) {
            number_start = sm->position;
        }
        
        sm->current_state = next_state;
        
        if (current_char != '\0') {
            sm->position++;
        }
    }
    
    return (sm->current_state == STATE_DONE) ? 0 : -1;
}

int main(void) {
    StateMachine sm;
    init_state_machine(&sm);
    
    printf("Enter binary numbers separated by whitespace: ");
    
    if (fgets(sm.input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    sm.input_len = strlen(sm.input_buffer);
    if (sm.input_len > 0 && sm.input_buffer[sm.input_len - 1] == '\n') {
        sm.input_buffer[sm.input_len - 1] = '\0';
        sm.input_len--;
    }
    
    if (sm.input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return
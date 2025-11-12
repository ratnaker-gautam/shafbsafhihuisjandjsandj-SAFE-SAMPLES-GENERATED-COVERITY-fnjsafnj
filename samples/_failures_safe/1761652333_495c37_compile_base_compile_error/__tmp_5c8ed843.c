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
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_OUTPUT, ' ', STATE_IDLE},
    {STATE_OUTPUT, '\n', STATE_IDLE},
    {STATE_OUTPUT, '\t', STATE_IDLE},
    {STATE_OUTPUT, '0', STATE_READING},
    {STATE_OUTPUT, '1', STATE_READING},
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

void process_binary_number(StateMachine* sm) {
    sm->result = 0;
    for (size_t i = 0; i < sm->input_len; i++) {
        if (sm->input_buffer[i] != '0' && sm->input_buffer[i] != '1') {
            sm->result = -1;
            return;
        }
        if (sm->result > (INT_MAX >> 1)) {
            sm->result = -1;
            return;
        }
        sm->result = (sm->result << 1) | (sm->input_buffer[i] - '0');
    }
}

void reset_machine(StateMachine* sm) {
    sm->current_state = STATE_IDLE;
    sm->input_len = 0;
    sm->position = 0;
    sm->result = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

int main(void) {
    StateMachine sm;
    reset_machine(&sm);
    
    char input_line[MAX_INPUT_LEN * 2];
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_line_len = strlen(input_line);
    if (input_line_len >= sizeof(input_line)) {
        return EXIT_FAILURE;
    }
    
    while (sm.position < input_line_len && sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        char current_char = input_line[sm.position];
        
        if (sm.current_state == STATE_IDLE && current_char != '0' && current_char != '1' && 
            current_char != ' ' && current_char != '\t' && current_char != '\n') {
            sm.current_state = STATE_ERROR;
            break;
        }
        
        State next_state = get_next_state(sm.current_state, current_char);
        
        if (next_state == STATE_ERROR) {
            sm.current_state = STATE_ERROR;
            break;
        }
        
        if (sm.current_state == STATE_READING && next_state == STATE_READING) {
            if (sm.input_len >= MAX_INPUT_LEN - 1) {
                sm.current_state = STATE_ERROR;
                break;
            }
            sm.input_buffer[sm.input_len++] = current_char;
        }
        
        if (next_state == STATE_PROCESSING && sm.current_state == STATE_READING) {
            if (sm.input_len > 0) {
                process_binary_number(&sm);
                if (sm.result == -1) {
                    sm.current_state = STATE_ERROR;
                    break;
                }
            }
        }
        
        if (next_state == STATE_OUTPUT && sm.current_state == STATE_PROCESSING) {
            if (sm.input_len > 0) {
                printf("%d\n", sm.result);
                sm.input_len = 0;
                sm.result = 0;
            }
        }
        
        sm.current_state = next_state;
        sm.position++;
        
        if (sm.current_state == STATE_IDLE && sm.input_len >
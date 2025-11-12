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

void process_binary_number(StateMachine *sm) {
    if (sm->input_len == 0 || sm->input_len > 63) {
        sm->result = 0;
        return;
    }
    
    uint64_t value = 0;
    for (size_t i = 0; i < sm->input_len; i++) {
        value <<= 1;
        if (sm->input_buffer[i] == '1') {
            value |= 1;
        }
    }
    sm->result = (int)(value & 0x7FFFFFFF);
}

void reset_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    sm->input_len = 0;
    sm->position = 0;
    sm->result = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
}

int run_state_machine(StateMachine *sm, const char *input) {
    reset_machine(sm);
    
    size_t input_length = strnlen(input, MAX_INPUT_LEN);
    if (input_length >= MAX_INPUT_LEN) {
        return -1;
    }
    
    while (sm->position <= input_length) {
        char current_char = input[sm->position];
        
        if (current_char != '0' && current_char != '1' && 
            current_char != ' ' && current_char != '\t' && 
            current_char != '\n' && current_char != '\0') {
            return -1;
        }
        
        State next_state = get_next_state(sm->current_state, current_char);
        
        switch (sm->current_state) {
            case STATE_READING:
                if (sm->input_len < MAX_INPUT_LEN - 1) {
                    sm->input_buffer[sm->input_len++] = current_char;
                } else {
                    return -1;
                }
                break;
                
            case STATE_PROCESSING:
                if (sm->input_len > 0) {
                    process_binary_number(sm);
                    sm->input_len = 0;
                }
                break;
                
            case STATE_OUTPUT:
                printf("%d\n", sm->result);
                break;
                
            case STATE_ERROR:
                return -1;
                
            default:
                break;
        }
        
        sm->current_state = next_state;
        sm->position++;
        
        if (sm->current_state == STATE_DONE) {
            break;
        }
    }
    
    return (sm->current_state == STATE_DONE) ? 0 : -1;
}

int main(void) {
    StateMachine sm;
    char input_line[MAX_INPUT_LEN + 2];
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strnlen(input_line, sizeof(input_line));
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
    }
    
    if (run_state_machine(&sm, input_line) != 0) {
        return 1;
    }
    
    return 0;
}
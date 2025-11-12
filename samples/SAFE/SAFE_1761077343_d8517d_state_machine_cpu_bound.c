//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    {STATE_IDLE, '\0', STATE_DONE},
    {STATE_IDLE, '0', STATE_READING},
    {STATE_IDLE, '1', STATE_READING},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, ' ', STATE_PROCESSING},
    {STATE_READING, '\n', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_PROCESSING},
    {STATE_PROCESSING, ' ', STATE_PROCESSING},
    {STATE_PROCESSING, '\n', STATE_PROCESSING},
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_OUTPUT, ' ', STATE_IDLE},
    {STATE_OUTPUT, '\n', STATE_IDLE},
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

int process_binary_number(StateMachine *sm) {
    int value = 0;
    size_t start = sm->position;
    
    while (start < sm->input_len) {
        char c = sm->input_buffer[start];
        if (c != '0' && c != '1') {
            break;
        }
        value = (value << 1) | (c - '0');
        start++;
    }
    
    sm->position = start;
    return value;
}

void run_state_machine(StateMachine *sm) {
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        char current_char = (sm->position < sm->input_len) ? sm->input_buffer[sm->position] : '\0';
        State next_state = get_next_state(sm->current_state, current_char);
        
        switch (sm->current_state) {
            case STATE_READING:
                if (next_state == STATE_PROCESSING) {
                    sm->result += process_binary_number(sm);
                } else {
                    sm->position++;
                }
                break;
                
            case STATE_OUTPUT:
                printf("%d\n", sm->result);
                sm->result = 0;
                sm->position++;
                break;
                
            case STATE_PROCESSING:
                sm->position++;
                break;
                
            default:
                sm->position++;
                break;
        }
        
        sm->current_state = next_state;
        
        if (sm->position > sm->input_len) {
            sm->current_state = STATE_ERROR;
        }
    }
}

int main(void) {
    StateMachine sm;
    char input_line[MAX_INPUT_LEN + 2];
    
    if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input_line);
    if (len > 0 && input_line[len - 1] == '\n') {
        input_line[len - 1] = '\0';
        len--;
    }
    
    if (len > MAX_INPUT_LEN) {
        return 1;
    }
    
    init_state_machine(&sm);
    memcpy(sm.input_buffer, input_line, len);
    sm.input_len = len;
    
    run_state_machine(&sm);
    
    if (sm.current_state == STATE_ERROR) {
        return 1;
    }
    
    return 0;
}
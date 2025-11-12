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
    size_t input_pos;
    char output_buffer[MAX_INPUT_LEN];
    size_t output_pos;
    int error_code;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, 'P', STATE_OUTPUT},
    {STATE_PROCESSING, '\0', STATE_ERROR},
    {STATE_OUTPUT, 'O', STATE_DONE},
    {STATE_OUTPUT, '\0', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->input_pos = 0;
    sm->output_pos = 0;
    sm->error_code = 0;
    memset(sm->input_buffer, 0, sizeof(sm->input_buffer));
    memset(sm->output_buffer, 0, sizeof(sm->output_buffer));
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void process_state(StateMachine *sm) {
    switch (sm->current_state) {
        case STATE_IDLE:
            if (sm->input_pos < MAX_INPUT_LEN - 1) {
                sm->input_buffer[sm->input_pos++] = 'S';
            }
            break;
        case STATE_READING:
            if (sm->input_pos < MAX_INPUT_LEN - 1) {
                sm->input_buffer[sm->input_pos++] = 'E';
            }
            break;
        case STATE_PROCESSING:
            if (sm->output_pos < MAX_INPUT_LEN - 1) {
                sm->output_buffer[sm->output_pos++] = 'P';
            }
            break;
        case STATE_OUTPUT:
            if (sm->output_pos < MAX_INPUT_LEN - 1) {
                sm->output_buffer[sm->output_pos++] = 'O';
            }
            break;
        case STATE_ERROR:
            sm->error_code = 1;
            break;
        case STATE_DONE:
            break;
    }
}

void simulate_processing(StateMachine *sm) {
    const int iterations = 1000000;
    volatile int result = 0;
    
    for (int i = 0; i < iterations; i++) {
        result += (i * 3) / 2;
        if (result > 1000000) {
            result = result % 1000;
        }
    }
    
    if (sm->output_pos < MAX_INPUT_LEN - 1) {
        sm->output_buffer[sm->output_pos++] = 'C';
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char sequence[] = "SEPO";
    size_t seq_len = strlen(sequence);
    
    for (size_t i = 0; i < seq_len; i++) {
        if (i >= MAX_INPUT_LEN - 1) break;
        
        char input_char = sequence[i];
        State next_state = get_next_state(sm.current_state, input_char);
        
        if (next_state == STATE_ERROR) {
            sm.current_state = STATE_ERROR;
            break;
        }
        
        sm.current_state = next_state;
        process_state(&sm);
        
        if (sm.current_state == STATE_PROCESSING) {
            simulate_processing(&sm);
        }
        
        if (sm.current_state == STATE_DONE) {
            break;
        }
    }
    
    if (sm.current_state == STATE_DONE) {
        printf("State machine completed successfully\n");
        printf("Input buffer: %s\n", sm.input_buffer);
        printf("Output buffer: %s\n", sm.output_buffer);
    } else if (sm.current_state == STATE_ERROR) {
        printf("State machine encountered an error\n");
        printf("Error code: %d\n", sm.error_code);
    } else {
        printf("State machine terminated in state: %d\n", sm.current_state);
    }
    
    return sm.current_state == STATE_DONE ? 0 : 1;
}
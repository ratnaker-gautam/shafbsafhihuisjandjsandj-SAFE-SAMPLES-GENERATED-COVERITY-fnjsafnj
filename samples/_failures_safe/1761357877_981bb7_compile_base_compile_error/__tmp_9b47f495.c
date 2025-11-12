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
    {STATE_OUTPUT, 'C', STATE_DONE},
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

void process_state(StateMachine *sm, char input) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == 'S') {
                state_machine_init(sm);
                sm->current_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (input == 'E') {
                sm->current_state = STATE_PROCESSING;
            } else if (isalnum((unsigned char)input) && sm->input_pos < MAX_INPUT_LEN - 1) {
                sm->input_buffer[sm->input_pos++] = input;
                sm->input_buffer[sm->input_pos] = '\0';
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 1;
            }
            break;
            
        case STATE_PROCESSING:
            if (input == 'P') {
                for (size_t i = 0; i < sm->input_pos && sm->output_pos < MAX_INPUT_LEN - 1; i++) {
                    char c = sm->input_buffer[i];
                    if (isalpha((unsigned char)c)) {
                        sm->output_buffer[sm->output_pos++] = toupper((unsigned char)c);
                    } else if (isdigit((unsigned char)c)) {
                        sm->output_buffer[sm->output_pos++] = c;
                    }
                }
                sm->output_buffer[sm->output_pos] = '\0';
                sm->current_state = STATE_OUTPUT;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 2;
            }
            break;
            
        case STATE_OUTPUT:
            if (input == 'C') {
                printf("Processed output: %s\n", sm->output_buffer);
                sm->current_state = STATE_DONE;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 3;
            }
            break;
            
        case STATE_ERROR:
            if (input == 'R') {
                state_machine_init(sm);
            }
            break;
            
        case STATE_DONE:
            if (input == 'S') {
                state_machine_init(sm);
                sm->current_state = STATE_READING;
            }
            break;
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input_sequence[] = "SHELLO123EPCPWORLD456EPC";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len > MAX_INPUT_LEN * 2) {
        fprintf(stderr, "Input sequence too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < seq_len; i++) {
        char current_char = input_sequence[i];
        
        if (!isprint((unsigned char)current_char)) {
            fprintf(stderr, "Invalid character in sequence\n");
            return 1;
        }
        
        State next_state = get_next_state(sm.current_state, current_char);
        
        if (next_state == STATE_ERROR && sm.current_state != STATE_ERROR) {
            fprintf(stderr, "Invalid transition from state %d with input '%c'\n", 
                   sm.current_state, current_char);
            process_state(&sm, '\0');
        } else {
            process_state(&sm, current_char);
        }
        
        if (sm.current
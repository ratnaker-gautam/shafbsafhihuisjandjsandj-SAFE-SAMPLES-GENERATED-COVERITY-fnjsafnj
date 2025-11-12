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
    char output_buffer[MAX_INPUT_LEN];
    size_t output_len;
    int error_code;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->output_len = 0;
    machine->error_code = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
    memset(machine->output_buffer, 0, MAX_INPUT_LEN);
}

int add_transition(Transition transitions[], int *count, State current, char input, State next) {
    if (*count >= MAX_TRANSITIONS) {
        return 0;
    }
    transitions[*count].current = current;
    transitions[*count].input_char = input;
    transitions[*count].next = next;
    (*count)++;
    return 1;
}

void initialize_transitions(Transition transitions[], int *count) {
    *count = 0;
    add_transition(transitions, count, STATE_IDLE, 'S', STATE_READING);
    add_transition(transitions, count, STATE_READING, 'E', STATE_PROCESSING);
    add_transition(transitions, count, STATE_READING, 'X', STATE_ERROR);
    add_transition(transitions, count, STATE_PROCESSING, 'P', STATE_OUTPUT);
    add_transition(transitions, count, STATE_PROCESSING, 'X', STATE_ERROR);
    add_transition(transitions, count, STATE_OUTPUT, 'O', STATE_DONE);
    add_transition(transitions, count, STATE_OUTPUT, 'X', STATE_ERROR);
    add_transition(transitions, count, STATE_ERROR, 'R', STATE_IDLE);
}

State get_next_state(Transition transitions[], int count, State current, char input) {
    for (int i = 0; i < count; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void process_state(StateMachine *machine, char input) {
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->input_len < MAX_INPUT_LEN - 1) {
                machine->input_buffer[machine->input_len++] = input;
            }
            break;
        case STATE_READING:
            if (machine->input_len < MAX_INPUT_LEN - 1) {
                machine->input_buffer[machine->input_len++] = input;
            }
            break;
        case STATE_PROCESSING:
            if (isalpha(input)) {
                if (machine->output_len < MAX_INPUT_LEN - 1) {
                    machine->output_buffer[machine->output_len++] = toupper(input);
                }
            }
            break;
        case STATE_OUTPUT:
            break;
        case STATE_ERROR:
            machine->error_code = 1;
            break;
        case STATE_DONE:
            break;
    }
}

int main(void) {
    StateMachine machine;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
    
    initialize_machine(&machine);
    initialize_transitions(transitions, &transition_count);
    
    char input_sequence[] = "SHELLOPWORLDX";
    size_t input_size = strlen(input_sequence);
    
    for (size_t i = 0; i < input_size; i++) {
        char current_input = input_sequence[i];
        State next_state = get_next_state(transitions, transition_count, machine.current_state, current_input);
        
        process_state(&machine, current_input);
        machine.current_state = next_state;
        
        if (machine.current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (machine.current_state == STATE_DONE) {
        printf("Processing completed successfully.\n");
        printf("Input: %s\n", machine.input_buffer);
        printf("Output: %s\n", machine.output_buffer);
    } else if (machine.current_state == STATE_ERROR) {
        printf("Error occurred during processing.\n");
    } else {
        printf("Processing incomplete.\n");
    }
    
    return machine.error_code;
}
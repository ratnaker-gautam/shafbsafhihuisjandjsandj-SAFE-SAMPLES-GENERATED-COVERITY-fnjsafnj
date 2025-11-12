//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 64

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7
} Input;

typedef struct {
    State current;
    Input input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    size_t transition_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
}

int add_transition(StateMachine *machine, State from, Input input, State to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

State process_input(StateMachine *machine, Input input) {
    if (input >= MAX_INPUTS) {
        return machine->current_state;
    }
    for (size_t i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            break;
        }
    }
    return machine->current_state;
}

void generate_input_sequence(uint8_t *sequence, size_t length) {
    for (size_t i = 0; i < length; i++) {
        sequence[i] = (uint8_t)(i % MAX_INPUTS);
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    for (State from = STATE_A; from < MAX_STATES; from++) {
        for (Input input = INPUT_0; input < MAX_INPUTS; input++) {
            State to = (State)((from + input) % MAX_STATES);
            if (!add_transition(&machine, from, input, to)) {
                fprintf(stderr, "Failed to add transition\n");
                return 1;
            }
        }
    }
    
    const size_t sequence_length = 1000000;
    uint8_t *input_sequence = malloc(sequence_length);
    if (input_sequence == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    generate_input_sequence(input_sequence, sequence_length);
    
    for (size_t i = 0; i < sequence_length; i++) {
        if (input_sequence[i] >= MAX_INPUTS) {
            fprintf(stderr, "Invalid input in sequence\n");
            free(input_sequence);
            return 1;
        }
        process_input(&machine, (Input)input_sequence[i]);
    }
    
    printf("Final state: %d\n", machine.current_state);
    free(input_sequence);
    return 0;
}
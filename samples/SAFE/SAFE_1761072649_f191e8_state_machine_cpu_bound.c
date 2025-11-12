//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 256

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} state_t;

typedef struct {
    state_t current;
    char input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    int transition_count;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
}

int add_transition(state_machine_t *machine, state_t from, char input, state_t to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES) {
        return 0;
    }
    
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

state_t process_input(state_machine_t *machine, char input) {
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            return machine->current_state;
        }
    }
    return machine->current_state;
}

void simulate_machine(state_machine_t *machine, const char *input_sequence) {
    if (input_sequence == NULL) {
        return;
    }
    
    size_t len = strnlen(input_sequence, MAX_INPUTS);
    if (len == 0) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        state_t new_state = process_input(machine, input_sequence[i]);
        printf("Input '%c': State %d -> State %d\n", 
               input_sequence[i], machine->current_state, new_state);
    }
}

int validate_input_string(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strnlen(str, MAX_INPUTS);
    if (len == 0 || len >= MAX_INPUTS) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] < 32 || str[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void setup_transitions(state_machine_t *machine) {
    add_transition(machine, STATE_A, '0', STATE_B);
    add_transition(machine, STATE_A, '1', STATE_C);
    add_transition(machine, STATE_B, '0', STATE_D);
    add_transition(machine, STATE_B, '1', STATE_E);
    add_transition(machine, STATE_C, '0', STATE_F);
    add_transition(machine, STATE_C, '1', STATE_G);
    add_transition(machine, STATE_D, '0', STATE_H);
    add_transition(machine, STATE_D, '1', STATE_A);
    add_transition(machine, STATE_E, '0', STATE_B);
    add_transition(machine, STATE_E, '1', STATE_C);
    add_transition(machine, STATE_F, '0', STATE_D);
    add_transition(machine, STATE_F, '1', STATE_E);
    add_transition(machine, STATE_G, '0', STATE_F);
    add_transition(machine, STATE_G, '1', STATE_G);
    add_transition(machine, STATE_H, '0', STATE_H);
    add_transition(machine, STATE_H, '1', STATE_A);
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    setup_transitions(&machine);
    
    char input_buffer[MAX_INPUTS + 1];
    printf("Enter input sequence (max %d characters, only printable ASCII): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strnlen(input_buffer, sizeof(input_buffer));
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input_string(input_buffer)) {
        fprintf(stderr, "Invalid input string\n");
        return EXIT_FAILURE;
    }
    
    printf("Initial state: %d\n", machine.current_state);
    simulate_machine(&machine, input_buffer);
    printf("Final state: %d\n", machine.current_state);
    
    return EXIT_SUCCESS;
}
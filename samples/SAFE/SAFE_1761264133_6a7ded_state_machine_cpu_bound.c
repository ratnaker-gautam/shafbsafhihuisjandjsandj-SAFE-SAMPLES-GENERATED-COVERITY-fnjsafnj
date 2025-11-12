//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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

void simulate_state_machine(state_machine_t *machine, const char *input_sequence) {
    if (input_sequence == NULL) {
        return;
    }
    
    size_t len = strnlen(input_sequence, MAX_INPUTS);
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
    for (size_t i = 0; i < len; i++) {
        if (str[i] < 'a' || str[i] > 'z') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    state_machine_t fsm;
    initialize_machine(&fsm);
    
    add_transition(&fsm, STATE_A, 'a', STATE_B);
    add_transition(&fsm, STATE_A, 'b', STATE_C);
    add_transition(&fsm, STATE_B, 'a', STATE_C);
    add_transition(&fsm, STATE_B, 'b', STATE_D);
    add_transition(&fsm, STATE_C, 'a', STATE_D);
    add_transition(&fsm, STATE_C, 'b', STATE_E);
    add_transition(&fsm, STATE_D, 'a', STATE_E);
    add_transition(&fsm, STATE_D, 'b', STATE_F);
    add_transition(&fsm, STATE_E, 'a', STATE_F);
    add_transition(&fsm, STATE_E, 'b', STATE_G);
    add_transition(&fsm, STATE_F, 'a', STATE_G);
    add_transition(&fsm, STATE_F, 'b', STATE_H);
    add_transition(&fsm, STATE_G, 'a', STATE_H);
    add_transition(&fsm, STATE_G, 'b', STATE_A);
    add_transition(&fsm, STATE_H, 'a', STATE_A);
    add_transition(&fsm, STATE_H, 'b', STATE_B);
    
    char input_buffer[MAX_INPUTS + 1];
    printf("Enter input sequence (a-z only, max %d chars): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(input_buffer, sizeof(input_buffer));
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input_string(input_buffer)) {
        fprintf(stderr, "Invalid input: only lowercase letters a-z allowed\n");
        return 1;
    }
    
    printf("Initial state: %d\n", fsm.current_state);
    simulate_state_machine(&fsm, input_buffer);
    printf("Final state: %d\n", fsm.current_state);
    
    return 0;
}
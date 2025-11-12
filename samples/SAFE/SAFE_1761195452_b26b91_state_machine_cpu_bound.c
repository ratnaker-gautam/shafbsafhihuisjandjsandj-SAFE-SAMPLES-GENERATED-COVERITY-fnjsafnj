//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

const char* state_name(state_t state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_F: return "F";
        case STATE_G: return "G";
        case STATE_H: return "H";
        default: return "UNKNOWN";
    }
}

void simulate_machine(state_machine_t *machine, const char *input_sequence) {
    if (input_sequence == NULL) {
        return;
    }
    
    size_t len = strlen(input_sequence);
    if (len > MAX_INPUTS) {
        return;
    }
    
    printf("Initial state: %s\n", state_name(machine->current_state));
    
    for (size_t i = 0; i < len; i++) {
        state_t new_state = process_input(machine, input_sequence[i]);
        printf("Input '%c': %s -> %s\n", input_sequence[i], 
               state_name(machine->transitions[i % machine->transition_count].current), 
               state_name(new_state));
    }
    
    printf("Final state: %s\n", state_name(machine->current_state));
}

int main() {
    state_machine_t machine;
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, '0', STATE_B);
    add_transition(&machine, STATE_A, '1', STATE_C);
    add_transition(&machine, STATE_B, '0', STATE_D);
    add_transition(&machine, STATE_B, '1', STATE_E);
    add_transition(&machine, STATE_C, '0', STATE_F);
    add_transition(&machine, STATE_C, '1', STATE_G);
    add_transition(&machine, STATE_D, '0', STATE_H);
    add_transition(&machine, STATE_D, '1', STATE_A);
    add_transition(&machine, STATE_E, '0', STATE_B);
    add_transition(&machine, STATE_E, '1', STATE_C);
    add_transition(&machine, STATE_F, '0', STATE_D);
    add_transition(&machine, STATE_F, '1', STATE_E);
    add_transition(&machine, STATE_G, '0', STATE_F);
    add_transition(&machine, STATE_G, '1', STATE_G);
    add_transition(&machine, STATE_H, '0', STATE_H);
    add_transition(&machine, STATE_H, '1', STATE_A);
    
    char input_buffer[MAX_INPUTS + 1];
    printf("Enter input sequence (max %d characters, 0s and 1s only): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (input_buffer[i] != '0' && input_buffer[i] != '1') {
            printf("Invalid input character: '%c'\n", input_buffer[i]);
            return 1;
        }
    }
    
    simulate_machine(&machine, input_buffer);
    
    return 0;
}
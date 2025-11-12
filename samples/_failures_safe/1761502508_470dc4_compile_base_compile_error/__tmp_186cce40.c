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
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H,
    STATE_I,
    STATE_J
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
}

int add_transition(StateMachine *machine, State from, char input, State to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

State process_input(StateMachine *machine, char input) {
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            return machine->current_state;
        }
    }
    return machine->current_state;
}

void build_sample_machine(StateMachine *machine) {
    add_transition(machine, STATE_A, '0', STATE_B);
    add_transition(machine, STATE_A, '1', STATE_C);
    add_transition(machine, STATE_B, '0', STATE_D);
    add_transition(machine, STATE_B, '1', STATE_E);
    add_transition(machine, STATE_C, '0', STATE_F);
    add_transition(machine, STATE_C, '1', STATE_G);
    add_transition(machine, STATE_D, '0', STATE_H);
    add_transition(machine, STATE_D, '1', STATE_I);
    add_transition(machine, STATE_E, '0', STATE_J);
    add_transition(machine, STATE_E, '1', STATE_A);
    add_transition(machine, STATE_F, '0', STATE_B);
    add_transition(machine, STATE_F, '1', STATE_C);
    add_transition(machine, STATE_G, '0', STATE_D);
    add_transition(machine, STATE_G, '1', STATE_E);
    add_transition(machine, STATE_H, '0', STATE_F);
    add_transition(machine, STATE_H, '1', STATE_G);
    add_transition(machine, STATE_I, '0', STATE_H);
    add_transition(machine, STATE_I, '1', STATE_I);
    add_transition(machine, STATE_J, '0', STATE_J);
    add_transition(machine, STATE_J, '1', STATE_A);
}

const char* state_to_string(State state) {
    switch(state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_F: return "F";
        case STATE_G: return "G";
        case STATE_H: return "H";
        case STATE_I: return "I";
        case STATE_J: return "J";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine machine;
    char input_buffer[MAX_INPUT_LEN + 1];
    
    initialize_machine(&machine);
    build_sample_machine(&machine);
    
    printf("State Machine Simulation (Binary Input)\n");
    printf("Current state: %s\n", state_to_string(machine.current_state));
    printf("Enter binary sequence (0s and 1s, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input_buffer[i] != '0' && input_buffer[i] != '1') {
            fprintf(stderr, "Invalid input: only 0 and 1 allowed\n");
            return 1;
        }
    }
    
    printf("Processing sequence: %s\n", input_buffer);
    printf("State transitions:\n");
    
    for (size_t i = 0; i < len; i++) {
        State previous = machine.current_state;
        State new_state = process_input(&machine, input_buffer[i]);
        printf("  %c: %s -> %s\n", input_buffer[i], state_to_string(previous), state_to_string(new_state));
        
        for (int j = 0; j < 10000; j++) {
            volatile int computation =
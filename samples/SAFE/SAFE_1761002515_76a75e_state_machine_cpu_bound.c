//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 4

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

typedef struct {
    State current;
    int input;
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

int add_transition(StateMachine *machine, State from, int input, State to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (input < 0 || input >= MAX_INPUTS) {
        return 0;
    }
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

int process_input(StateMachine *machine, int input) {
    if (input < 0 || input >= MAX_INPUTS) {
        return 0;
    }
    
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].input == input) {
            machine->current_state = machine->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

const char* state_to_string(State state) {
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

int main() {
    StateMachine machine;
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, 0, STATE_B);
    add_transition(&machine, STATE_A, 1, STATE_C);
    add_transition(&machine, STATE_B, 0, STATE_D);
    add_transition(&machine, STATE_B, 1, STATE_E);
    add_transition(&machine, STATE_C, 0, STATE_F);
    add_transition(&machine, STATE_C, 1, STATE_G);
    add_transition(&machine, STATE_D, 0, STATE_H);
    add_transition(&machine, STATE_D, 1, STATE_A);
    add_transition(&machine, STATE_E, 0, STATE_B);
    add_transition(&machine, STATE_E, 1, STATE_C);
    add_transition(&machine, STATE_F, 0, STATE_D);
    add_transition(&machine, STATE_F, 1, STATE_E);
    add_transition(&machine, STATE_G, 0, STATE_F);
    add_transition(&machine, STATE_G, 1, STATE_G);
    add_transition(&machine, STATE_H, 0, STATE_H);
    add_transition(&machine, STATE_H, 1, STATE_A);
    
    printf("State Machine Simulation\n");
    printf("Valid inputs: 0, 1, 2, 3\n");
    printf("Enter -1 to exit\n");
    printf("Current state: %s\n", state_to_string(machine.current_state));
    
    int input;
    while (1) {
        printf("Enter input: ");
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input format\n");
            continue;
        }
        
        if (input == -1) {
            break;
        }
        
        if (input < 0 || input >= MAX_INPUTS) {
            printf("Invalid input value\n");
            continue;
        }
        
        if (process_input(&machine, input)) {
            printf("Transition successful. Current state: %s\n", 
                   state_to_string(machine.current_state));
        } else {
            printf("No transition defined for state %s with input %d\n",
                   state_to_string(machine.current_state), input);
        }
    }
    
    printf("Final state: %s\n", state_to_string(machine.current_state));
    return 0;
}
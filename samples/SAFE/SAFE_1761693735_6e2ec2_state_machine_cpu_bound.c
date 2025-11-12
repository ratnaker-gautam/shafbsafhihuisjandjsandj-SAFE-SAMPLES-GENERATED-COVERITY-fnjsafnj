//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 128

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

int add_transition(StateMachine *machine, State current, Input input, State next) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (current >= MAX_STATES || next >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    machine->transitions[machine->transition_count].current = current;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = next;
    machine->transition_count++;
    return 1;
}

int process_input(StateMachine *machine, Input input) {
    if (input >= MAX_INPUTS) {
        return 0;
    }
    for (size_t i = 0; i < machine->transition_count; i++) {
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

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);

    add_transition(&machine, STATE_A, INPUT_0, STATE_B);
    add_transition(&machine, STATE_A, INPUT_1, STATE_C);
    add_transition(&machine, STATE_B, INPUT_0, STATE_D);
    add_transition(&machine, STATE_B, INPUT_1, STATE_E);
    add_transition(&machine, STATE_C, INPUT_0, STATE_F);
    add_transition(&machine, STATE_C, INPUT_1, STATE_G);
    add_transition(&machine, STATE_D, INPUT_0, STATE_H);
    add_transition(&machine, STATE_D, INPUT_1, STATE_A);
    add_transition(&machine, STATE_E, INPUT_0, STATE_B);
    add_transition(&machine, STATE_E, INPUT_1, STATE_C);
    add_transition(&machine, STATE_F, INPUT_0, STATE_D);
    add_transition(&machine, STATE_F, INPUT_1, STATE_E);
    add_transition(&machine, STATE_G, INPUT_0, STATE_F);
    add_transition(&machine, STATE_G, INPUT_1, STATE_G);
    add_transition(&machine, STATE_H, INPUT_0, STATE_H);
    add_transition(&machine, STATE_H, INPUT_1, STATE_A);

    printf("State Machine Simulation\n");
    printf("Current state: %s\n", state_to_string(machine.current_state));
    printf("Enter inputs (0-7, -1 to exit): ");

    int input_val;
    while (1) {
        if (scanf("%d", &input_val) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        if (input_val == -1) {
            break;
        }
        if (input_val < 0 || input_val >= MAX_INPUTS) {
            printf("Input out of range (0-7)\n");
            continue;
        }
        if (!process_input(&machine, (Input)input_val)) {
            printf("No transition for state %s with input %d\n", 
                   state_to_string(machine.current_state), input_val);
        } else {
            printf("New state: %s\n", state_to_string(machine.current_state));
        }
    }

    printf("Final state: %s\n", state_to_string(machine.current_state));
    return 0;
}
//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    INPUT_INVALID
} Input;

typedef struct {
    State current;
    Input input;
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

int add_transition(StateMachine *machine, State from, Input input, State to) {
    if (machine->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= INPUT_INVALID) {
        return 0;
    }
    machine->transitions[machine->transition_count].current = from;
    machine->transitions[machine->transition_count].input = input;
    machine->transitions[machine->transition_count].next = to;
    machine->transition_count++;
    return 1;
}

Input parse_input(char c) {
    switch (c) {
        case '0': return INPUT_0;
        case '1': return INPUT_1;
        case '2': return INPUT_2;
        case '3': return INPUT_3;
        default: return INPUT_INVALID;
    }
}

State process_input(StateMachine *machine, Input input) {
    if (input == INPUT_INVALID) {
        return machine->current_state;
    }
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].input == input) {
            return machine->transitions[i].next;
        }
    }
    return machine->current_state;
}

void run_state_machine(StateMachine *machine, const char *input_sequence) {
    if (input_sequence == NULL) {
        return;
    }
    size_t len = strlen(input_sequence);
    if (len > MAX_INPUTS) {
        return;
    }
    for (size_t i = 0; i < len; i++) {
        Input input = parse_input(input_sequence[i]);
        machine->current_state = process_input(machine, input);
    }
}

int main() {
    StateMachine machine;
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, INPUT_0, STATE_B);
    add_transition(&machine, STATE_A, INPUT_1, STATE_C);
    add_transition(&machine, STATE_B, INPUT_0, STATE_C);
    add_transition(&machine, STATE_B, INPUT_1, STATE_D);
    add_transition(&machine, STATE_C, INPUT_0, STATE_D);
    add_transition(&machine, STATE_C, INPUT_1, STATE_E);
    add_transition(&machine, STATE_D, INPUT_0, STATE_E);
    add_transition(&machine, STATE_D, INPUT_1, STATE_F);
    add_transition(&machine, STATE_E, INPUT_0, STATE_F);
    add_transition(&machine, STATE_E, INPUT_1, STATE_G);
    add_transition(&machine, STATE_F, INPUT_0, STATE_G);
    add_transition(&machine, STATE_F, INPUT_1, STATE_H);
    add_transition(&machine, STATE_G, INPUT_0, STATE_H);
    add_transition(&machine, STATE_G, INPUT_1, STATE_A);
    add_transition(&machine, STATE_H, INPUT_0, STATE_A);
    add_transition(&machine, STATE_H, INPUT_1, STATE_B);
    
    char input_buffer[32];
    printf("Enter input sequence (0-3 only, max %d chars): ", MAX_INPUTS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
    }
    
    run_state_machine(&machine, input_buffer);
    
    printf("Final state: ");
    switch (machine.current_state) {
        case STATE_A: printf("A\n"); break;
        case STATE_B: printf("B\n"); break;
        case STATE_C: printf("C\n"); break;
        case STATE_D: printf("D\n"); break;
        case STATE_E: printf("E\n"); break;
        case STATE_F: printf("F\n"); break;
        case STATE_G: printf("G\n"); break;
        case STATE_H: printf("H\n"); break;
    }
    
    return 0;
}
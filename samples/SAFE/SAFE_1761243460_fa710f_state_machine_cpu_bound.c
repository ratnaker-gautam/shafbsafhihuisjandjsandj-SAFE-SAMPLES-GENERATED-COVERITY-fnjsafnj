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

void simulate_computation(int iterations) {
    volatile uint64_t result = 0;
    for (int i = 0; i < iterations; i++) {
        result = (result * 1103515245 + 12345) & 0x7FFFFFFF;
        for (int j = 0; j < 100; j++) {
            result ^= (result >> 13);
            result ^= (result << 17);
            result ^= (result >> 5);
        }
    }
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

int main() {
    state_machine_t fsm;
    char input_sequence[MAX_INPUTS];
    int sequence_length;
    
    printf("Enter sequence length (1-100): ");
    if (scanf("%d", &sequence_length) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (sequence_length < 1 || sequence_length > 100) {
        printf("Sequence length must be between 1 and 100\n");
        return 1;
    }
    
    printf("Enter %d characters (a-z): ", sequence_length);
    if (scanf("%s", input_sequence) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (strlen(input_sequence) != (size_t)sequence_length) {
        printf("Input length mismatch\n");
        return 1;
    }
    
    for (int i = 0; i < sequence_length; i++) {
        if (input_sequence[i] < 'a' || input_sequence[i] > 'z') {
            printf("Invalid character: %c\n", input_sequence[i]);
            return 1;
        }
    }
    
    initialize_machine(&fsm);
    
    add_transition(&fsm, STATE_A, 'a', STATE_B);
    add_transition(&fsm, STATE_A, 'b', STATE_C);
    add_transition(&fsm, STATE_B, 'c', STATE_D);
    add_transition(&fsm, STATE_B, 'd', STATE_E);
    add_transition(&fsm, STATE_C, 'e', STATE_F);
    add_transition(&fsm, STATE_C, 'f', STATE_G);
    add_transition(&fsm, STATE_D, 'g', STATE_H);
    add_transition(&fsm, STATE_E, 'h', STATE_A);
    add_transition(&fsm, STATE_F, 'i', STATE_B);
    add_transition(&fsm, STATE_G, 'j', STATE_C);
    add_transition(&fsm, STATE_H, 'k', STATE_D);
    
    printf("Initial state: %s\n", state_name(fsm.current_state));
    
    for (int i = 0; i < sequence_length; i++) {
        state_t previous = fsm.current_state;
        state_t new_state = process_input(&fsm, input_sequence[i]);
        
        simulate_computation(1000);
        
        printf("Input: %c | State: %s -> %s\n", 
               input_sequence[i], state_name(previous), state_name(new_state));
    }
    
    printf("Final state: %s\n", state_name(fsm.current_state));
    
    return 0;
}
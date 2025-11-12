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

void simulate_machine(StateMachine *machine, const char *input) {
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        State previous = machine->current_state;
        State new_state = process_input(machine, input[i]);
        printf("Input '%c': State %d -> %d\n", input[i], previous, new_state);
        
        for (int j = 0; j < 1000000; j++) {
            volatile int x = j * j;
            (void)x;
        }
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    add_transition(&machine, STATE_A, '0', STATE_B);
    add_transition(&machine, STATE_A, '1', STATE_C);
    add_transition(&machine, STATE_B, '0', STATE_D);
    add_transition(&machine, STATE_B, '1', STATE_E);
    add_transition(&machine, STATE_C, '0', STATE_F);
    add_transition(&machine, STATE_C, '1', STATE_G);
    add_transition(&machine, STATE_D, '0', STATE_H);
    add_transition(&machine, STATE_D, '1', STATE_I);
    add_transition(&machine, STATE_E, '0', STATE_J);
    add_transition(&machine, STATE_E, '1', STATE_A);
    add_transition(&machine, STATE_F, '0', STATE_B);
    add_transition(&machine, STATE_F, '1', STATE_C);
    add_transition(&machine, STATE_G, '0', STATE_D);
    add_transition(&machine, STATE_G, '1', STATE_E);
    add_transition(&machine, STATE_H, '0', STATE_F);
    add_transition(&machine, STATE_H, '1', STATE_G);
    add_transition(&machine, STATE_I, '0', STATE_H);
    add_transition(&machine, STATE_I, '1', STATE_I);
    add_transition(&machine, STATE_J, '0', STATE_J);
    add_transition(&machine, STATE_J, '1', STATE_A);
    
    char input[MAX_INPUT_LEN + 1];
    printf("Enter binary input sequence (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            printf("Invalid input character '%c'. Only '0' and '1' allowed.\n", input[i]);
            return 1;
        }
    }
    
    printf("Starting state: %d\n", machine.current_state);
    printf("Processing input: %s\n", input);
    
    simulate_machine(&machine, input);
    
    printf("Final state: %d\n", machine.current_state);
    
    return 0;
}
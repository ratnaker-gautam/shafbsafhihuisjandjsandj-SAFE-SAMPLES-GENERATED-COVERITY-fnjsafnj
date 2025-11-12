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

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_A;
    sm->transition_count = 0;
}

int add_transition(StateMachine *sm, State from, Input input, State to) {
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= INPUT_INVALID) {
        return 0;
    }
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

Input get_input(char c) {
    switch (c) {
        case '0': return INPUT_0;
        case '1': return INPUT_1;
        case '2': return INPUT_2;
        case '3': return INPUT_3;
        default: return INPUT_INVALID;
    }
}

State process_transition(const StateMachine *sm, Input input) {
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            return sm->transitions[i].next;
        }
    }
    return sm->current_state;
}

void run_state_machine(StateMachine *sm, const char *input_sequence) {
    size_t len = strlen(input_sequence);
    if (len > MAX_INPUTS) {
        printf("Input sequence too long\n");
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        Input input = get_input(input_sequence[i]);
        if (input == INPUT_INVALID) {
            printf("Invalid input character: %c\n", input_sequence[i]);
            continue;
        }
        
        State next_state = process_transition(sm, input);
        sm->current_state = next_state;
        
        printf("Step %zu: Input '%c' -> State %d\n", 
               i + 1, input_sequence[i], sm->current_state);
    }
}

void setup_transitions(StateMachine *sm) {
    add_transition(sm, STATE_A, INPUT_0, STATE_B);
    add_transition(sm, STATE_A, INPUT_1, STATE_C);
    add_transition(sm, STATE_B, INPUT_0, STATE_D);
    add_transition(sm, STATE_B, INPUT_1, STATE_A);
    add_transition(sm, STATE_C, INPUT_0, STATE_A);
    add_transition(sm, STATE_C, INPUT_1, STATE_D);
    add_transition(sm, STATE_D, INPUT_0, STATE_E);
    add_transition(sm, STATE_D, INPUT_1, STATE_F);
    add_transition(sm, STATE_E, INPUT_0, STATE_G);
    add_transition(sm, STATE_E, INPUT_1, STATE_H);
    add_transition(sm, STATE_F, INPUT_0, STATE_H);
    add_transition(sm, STATE_F, INPUT_1, STATE_G);
    add_transition(sm, STATE_G, INPUT_0, STATE_A);
    add_transition(sm, STATE_G, INPUT_1, STATE_B);
    add_transition(sm, STATE_H, INPUT_0, STATE_C);
    add_transition(sm, STATE_H, INPUT_1, STATE_D);
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    setup_transitions(&sm);
    
    char input_buffer[MAX_INPUTS + 1];
    printf("Enter input sequence (0-3 only, max %d characters): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    run_state_machine(&sm, input_buffer);
    
    printf("Final state: %d\n", sm.current_state);
    
    return 0;
}
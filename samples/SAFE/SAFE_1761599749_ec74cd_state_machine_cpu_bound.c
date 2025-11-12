//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, Input input) {
    if (input >= MAX_INPUTS) {
        return sm->current_state;
    }
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    return sm->current_state;
}

int validate_input_string(const char *str) {
    if (str == NULL) {
        return 0;
    }
    size_t len = strlen(str);
    if (len == 0 || len > 100) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '7') {
            return 0;
        }
    }
    return 1;
}

Input char_to_input(char c) {
    if (c >= '0' && c <= '7') {
        return (Input)(c - '0');
    }
    return INPUT_0;
}

void setup_transitions(StateMachine *sm) {
    add_transition(sm, STATE_A, INPUT_0, STATE_B);
    add_transition(sm, STATE_A, INPUT_1, STATE_C);
    add_transition(sm, STATE_B, INPUT_2, STATE_D);
    add_transition(sm, STATE_B, INPUT_3, STATE_E);
    add_transition(sm, STATE_C, INPUT_4, STATE_F);
    add_transition(sm, STATE_C, INPUT_5, STATE_G);
    add_transition(sm, STATE_D, INPUT_6, STATE_H);
    add_transition(sm, STATE_E, INPUT_7, STATE_A);
    add_transition(sm, STATE_F, INPUT_0, STATE_B);
    add_transition(sm, STATE_G, INPUT_1, STATE_C);
    add_transition(sm, STATE_H, INPUT_2, STATE_D);
}

const char *state_to_string(State s) {
    switch (s) {
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
    StateMachine sm;
    char input_str[1024];
    
    initialize_machine(&sm);
    setup_transitions(&sm);
    
    printf("Enter input sequence (digits 0-7): ");
    if (fgets(input_str, sizeof(input_str), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_str);
    if (len > 0 && input_str[len - 1] == '\n') {
        input_str[len - 1] = '\0';
    }
    
    if (!validate_input_string(input_str)) {
        fprintf(stderr, "Invalid input sequence\n");
        return 1;
    }
    
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    for (size_t i = 0; input_str[i] != '\0'; i++) {
        Input input = char_to_input(input_str[i]);
        State new_state = process_input(&sm, input);
        printf("Input: %c -> State: %s\n", input_str[i], state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(sm.current_state));
    
    return 0;
}
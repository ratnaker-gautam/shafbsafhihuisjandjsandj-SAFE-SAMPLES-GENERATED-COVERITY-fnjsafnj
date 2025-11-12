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

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_A;
    sm->transition_count = 0;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine *sm, char input) {
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void setup_transitions(StateMachine *sm) {
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_E);
    add_transition(sm, STATE_C, '0', STATE_F);
    add_transition(sm, STATE_C, '1', STATE_G);
    add_transition(sm, STATE_D, '0', STATE_H);
    add_transition(sm, STATE_D, '1', STATE_I);
    add_transition(sm, STATE_E, '0', STATE_J);
    add_transition(sm, STATE_E, '1', STATE_A);
    add_transition(sm, STATE_F, '0', STATE_B);
    add_transition(sm, STATE_F, '1', STATE_C);
    add_transition(sm, STATE_G, '0', STATE_D);
    add_transition(sm, STATE_G, '1', STATE_E);
    add_transition(sm, STATE_H, '0', STATE_F);
    add_transition(sm, STATE_H, '1', STATE_G);
    add_transition(sm, STATE_I, '0', STATE_H);
    add_transition(sm, STATE_I, '1', STATE_I);
    add_transition(sm, STATE_J, '0', STATE_J);
    add_transition(sm, STATE_J, '1', STATE_A);
}

const char* state_to_string(State s) {
    switch(s) {
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
    StateMachine sm;
    char input_buffer[MAX_INPUT_LEN + 1];
    
    initialize_machine(&sm);
    setup_transitions(&sm);
    
    printf("Enter binary sequence (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    printf("Processing sequence: %s\n", input_buffer);
    
    for (size_t i = 0; i < len; i++) {
        char c = input_buffer[i];
        
        if (c != '0' && c != '1') {
            fprintf(stderr, "Invalid character '%c' at position %zu\n", c, i);
            return 1;
        }
        
        if (!process_input(&sm, c)) {
            fprintf(stderr, "No transition for state %s with input '%c'\n", 
                    state_to_string(sm.current_state), c);
            return 1;
        }
        
        printf("Input '%c' -> State: %s\n", c, state_to_string(sm.current_state));
    }
    
    printf("Final state: %s\n", state_to_string(s
//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUT_LEN 64

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
    char input;
    State next;
} Transition;

typedef struct {
    State *states;
    Transition *transitions;
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->states = malloc(MAX_STATES * sizeof(State));
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->states || !sm->transitions) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->state_count = 0;
    sm->transition_count = 0;
    sm->current_state = STATE_A;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

int add_state(StateMachine *sm, State state) {
    if (!sm || sm->state_count >= MAX_STATES) return 0;
    sm->states[sm->state_count++] = state;
    return 1;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    Transition *t = &sm->transitions[sm->transition_count++];
    t->current = from;
    t->input = input;
    t->next = to;
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && t->input == input) {
            sm->current_state = t->next;
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
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    for (State s = STATE_A; s <= STATE_H; s++) {
        if (!add_state(sm, s)) {
            fprintf(stderr, "Failed to add state\n");
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_E);
    add_transition(sm, STATE_C, '0', STATE_F);
    add_transition(sm, STATE_C, '1', STATE_G);
    add_transition(sm, STATE_D, '0', STATE_H);
    add_transition(sm, STATE_D, '1', STATE_A);
    add_transition(sm, STATE_E, '0', STATE_B);
    add_transition(sm, STATE_E, '1', STATE_C);
    add_transition(sm, STATE_F, '0', STATE_D);
    add_transition(sm, STATE_F, '1', STATE_E);
    add_transition(sm, STATE_G, '0', STATE_F);
    add_transition(sm, STATE_G, '1', STATE_G);
    add_transition(sm, STATE_H, '0', STATE_H);
    add_transition(sm, STATE_H, '1', STATE_A);
    
    char input_buffer[MAX_INPUT_LEN];
    printf("Enter binary sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    for (size_t i = 0; i < len; i++) {
        char c = input_buffer[i];
        if (c != '0' && c != '1') {
            printf("Invalid input '%c' at position %zu\n", c, i);
//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef struct {
    int id;
    char name[32];
} State;

typedef struct {
    int from_state;
    int to_state;
    char input_char;
} Transition;

typedef struct {
    State *states;
    Transition *transitions;
    int current_state;
    int state_count;
    int transition_count;
} StateMachine;

StateMachine* create_state_machine(void) {
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
    sm->current_state = -1;
    return sm;
}

int add_state(StateMachine *sm, int id, const char *name) {
    if (!sm || id < 0 || !name || sm->state_count >= MAX_STATES) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == id) return 0;
    }
    
    State *state = &sm->states[sm->state_count];
    state->id = id;
    strncpy(state->name, name, sizeof(state->name) - 1);
    state->name[sizeof(state->name) - 1] = '\0';
    sm->state_count++;
    
    if (sm->current_state == -1) {
        sm->current_state = id;
    }
    
    return 1;
}

int add_transition(StateMachine *sm, int from, int to, char input) {
    if (!sm || from < 0 || to < 0 || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_exists = 1;
        if (sm->states[i].id == to) to_exists = 1;
    }
    
    if (!from_exists || !to_exists) return 0;
    
    Transition *trans = &sm->transitions[sm->transition_count];
    trans->from_state = from;
    trans->to_state = to;
    trans->input_char = input;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *trans = &sm->transitions[i];
        if (trans->from_state == sm->current_state && trans->input_char == input) {
            sm->current_state = trans->to_state;
            return 1;
        }
    }
    return 0;
}

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    free(sm->states);
    free(sm->transitions);
    free(sm);
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    int states_stack[3] = {0, 1, 2};
    const char *state_names[] = {"START", "MIDDLE", "END"};
    
    for (int i = 0; i < 3; i++) {
        if (!add_state(sm, states_stack[i], state_names[i])) {
            fprintf(stderr, "Failed to add state %d\n", states_stack[i]);
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    struct {
        int from, to;
        char input;
    } transitions_heap[] = {
        {0, 1, 'a'},
        {1, 2, 'b'},
        {2, 0, 'c'}
    };
    
    for (size_t i = 0; i < sizeof(transitions_heap)/sizeof(transitions_heap[0]); i++) {
        if (!add_transition(sm, transitions_heap[i].from, transitions_heap[i].to, transitions_heap[i].input)) {
            fprintf(stderr, "Failed to add transition\n");
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    printf("State machine initialized. Current state: %s\n", sm->states[0].name);
    printf("Enter input characters (a, b, c) or 'q' to quit: ");
    
    char input[MAX_INPUT_LEN];
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Input error\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input) && input[i] != '\n' && input[i] != 'q'; i++) {
        if (process_input(sm, input[i])) {
            for (int j = 0; j < sm->
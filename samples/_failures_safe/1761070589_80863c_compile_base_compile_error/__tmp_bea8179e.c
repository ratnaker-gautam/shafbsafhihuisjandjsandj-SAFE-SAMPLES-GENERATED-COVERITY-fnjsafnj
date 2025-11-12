//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    char trigger;
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

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

int add_state(StateMachine *sm, int id, const char *name) {
    if (!sm || sm->state_count >= MAX_STATES || id < 0) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == id) return 0;
    }
    
    State *state = &sm->states[sm->state_count];
    state->id = id;
    strncpy(state->name, name, sizeof(state->name) - 1);
    state->name[sizeof(state->name) - 1] = '\0';
    
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine *sm, int from, int to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    int from_found = 0, to_found = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_found = 1;
        if (sm->states[i].id == to) to_found = 1;
    }
    
    if (!from_found || !to_found) return 0;
    
    Transition *trans = &sm->transitions[sm->transition_count];
    trans->from_state = from;
    trans->to_state = to;
    trans->trigger = trigger;
    
    sm->transition_count++;
    return 1;
}

int set_initial_state(StateMachine *sm, int state_id) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == state_id) {
            sm->current_state = state_id;
            return 1;
        }
    }
    return 0;
}

int process_input(StateMachine *sm, char input) {
    if (!sm || sm->current_state == -1) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].trigger == input) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

const char* get_current_state_name(StateMachine *sm) {
    if (!sm || sm->current_state == -1) return "INVALID";
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            return sm->states[i].name;
        }
    }
    return "INVALID";
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    int stack_states[3] = {0, 1, 2};
    char *state_names[] = {"START", "PROCESSING", "END"};
    
    for (int i = 0; i < 3; i++) {
        if (!add_state(sm, stack_states[i], state_names[i])) {
            fprintf(stderr, "Failed to add state %d\n", stack_states[i]);
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    if (!set_initial_state(sm, 0)) {
        fprintf(stderr, "Failed to set initial state\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    char transitions[][3] = {{0, 1, 's'}, {1, 2, 'e'}, {1, 0, 'r'}};
    
    for (int i = 0; i < 3; i++) {
        if (!add_transition(sm, transitions[i][0], transitions[i][1], transitions[i][2])) {
            fprintf(stderr, "Failed to add transition\n
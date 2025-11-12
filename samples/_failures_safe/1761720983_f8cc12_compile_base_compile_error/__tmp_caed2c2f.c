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
    char trigger;
} Transition;

typedef struct {
    State *states;
    Transition *transitions;
    int current_state;
    int num_states;
    int num_transitions;
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
    
    sm->num_states = 0;
    sm->num_transitions = 0;
    sm->current_state = -1;
    return sm;
}

int add_state(StateMachine *sm, int id, const char *name) {
    if (!sm || !name || id < 0 || sm->num_states >= MAX_STATES) return 0;
    
    for (int i = 0; i < sm->num_states; i++) {
        if (sm->states[i].id == id) return 0;
    }
    
    State *state = &sm->states[sm->num_states];
    state->id = id;
    strncpy(state->name, name, sizeof(state->name) - 1);
    state->name[sizeof(state->name) - 1] = '\0';
    
    sm->num_states++;
    if (sm->current_state == -1) {
        sm->current_state = id;
    }
    return 1;
}

int add_transition(StateMachine *sm, int from, int to, char trigger) {
    if (!sm || from < 0 || to < 0 || sm->num_transitions >= MAX_TRANSITIONS) return 0;
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < sm->num_states; i++) {
        if (sm->states[i].id == from) from_exists = 1;
        if (sm->states[i].id == to) to_exists = 1;
    }
    if (!from_exists || !to_exists) return 0;
    
    Transition *trans = &sm->transitions[sm->num_transitions];
    trans->from_state = from;
    trans->to_state = to;
    trans->trigger = trigger;
    
    sm->num_transitions++;
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].trigger == input) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

void print_current_state(StateMachine *sm) {
    if (!sm) return;
    
    for (int i = 0; i < sm->num_states; i++) {
        if (sm->states[i].id == sm->current_state) {
            printf("Current state: %s (ID: %d)\n", sm->states[i].name, sm->current_state);
            return;
        }
    }
}

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    free(sm->states);
    free(sm->transitions);
    free(sm);
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_state(sm, 0, "START");
    add_state(sm, 1, "PROCESSING");
    add_state(sm, 2, "VALIDATING");
    add_state(sm, 3, "COMPLETE");
    add_state(sm, 4, "ERROR");
    
    add_transition(sm, 0, 1, 's');
    add_transition(sm, 1, 2, 'p');
    add_transition(sm, 2, 3, 'v');
    add_transition(sm, 2, 4, 'e');
    add_transition(sm, 1, 4, 'e');
    add_transition(sm, 4, 0, 'r');
    add_transition(sm, 3, 0, 'r');
    
    printf("State Machine Ready. Commands: s(start), p(process), v(validate), e(error), r(reset), q(quit)\n");
    
    char input[MAX_INPUT_LEN];
    while (1) {
        print_current_state(sm);
        printf("Enter command: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }
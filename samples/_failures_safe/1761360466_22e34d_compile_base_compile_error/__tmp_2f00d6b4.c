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
    if (sm->current_state == -1) {
        sm->current_state = id;
    }
    return 1;
}

int add_transition(StateMachine *sm, int from, int to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS || from < 0 || to < 0) return 0;
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_exists = 1;
        if (sm->states[i].id == to) to_exists = 1;
    }
    if (!from_exists || !to_exists) return 0;
    
    Transition *trans = &sm->transitions[sm->transition_count];
    trans->from_state = from;
    trans->to_state = to;
    trans->trigger = trigger;
    
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *trans = &sm->transitions[i];
        if (trans->from_state == sm->current_state && trans->trigger == input) {
            sm->current_state = trans->to_state;
            return 1;
        }
    }
    return 0;
}

const char* get_current_state_name(StateMachine *sm) {
    if (!sm) return "INVALID";
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            return sm->states[i].name;
        }
    }
    return "UNKNOWN";
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_state(sm, 0, "START");
    add_state(sm, 1, "READING");
    add_state(sm, 2, "PROCESSING");
    add_state(sm, 3, "FINISHED");
    
    add_transition(sm, 0, 1, 'r');
    add_transition(sm, 1, 2, 'p');
    add_transition(sm, 2, 3, 'f');
    add_transition(sm, 1, 1, 'c');
    add_transition(sm, 2, 2, 'c');
    
    printf("State Machine Ready. Current state: %s\n", get_current_state_name(sm));
    printf("Valid inputs: r (read), p (process), f (finish), c (continue)\n");
    
    char input[MAX_INPUT_LEN];
    while (sm->current_state != 3) {
        printf("Enter input: ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        if (strlen(input) == 0 || input[0] == '\n') {
            continue;
        }
        
        char cmd = input[0];
        if (process_input(sm, cmd)) {
            printf("Transition successful. Current state: %s\n", get_current_state_name(sm));
        } else {
            printf("Invalid transition from state %s with input '%c'\n", get_current_state_name(sm), cmd
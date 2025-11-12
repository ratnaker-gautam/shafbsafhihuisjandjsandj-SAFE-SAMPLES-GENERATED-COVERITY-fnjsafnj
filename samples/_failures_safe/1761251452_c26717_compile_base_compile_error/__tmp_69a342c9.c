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

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

int add_state(StateMachine *sm, int id, const char *name) {
    if (!sm || id < 0 || !name || sm->num_states >= MAX_STATES) return 0;
    
    for (int i = 0; i < sm->num_states; i++) {
        if (sm->states[i].id == id) return 0;
    }
    
    sm->states[sm->num_states].id = id;
    strncpy(sm->states[sm->num_states].name, name, 31);
    sm->states[sm->num_states].name[31] = '\0';
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
    
    sm->transitions[sm->num_transitions].from_state = from;
    sm->transitions[sm->num_transitions].to_state = to;
    sm->transitions[sm->num_transitions].trigger = trigger;
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

const char* get_current_state_name(StateMachine *sm) {
    if (!sm) return "INVALID";
    
    for (int i = 0; i < sm->num_states; i++) {
        if (sm->states[i].id == sm->current_state) {
            return sm->states[i].name;
        }
    }
    return "UNKNOWN";
}

int main() {
    State stack_states[3];
    Transition stack_transitions[4];
    int stack_state_count = 0;
    int stack_transition_count = 0;
    
    StateMachine *heap_sm = create_state_machine();
    if (!heap_sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_state(heap_sm, 0, "START");
    add_state(heap_sm, 1, "PROCESSING");
    add_state(heap_sm, 2, "COMPLETE");
    add_state(heap_sm, 3, "ERROR");
    
    add_transition(heap_sm, 0, 1, 's');
    add_transition(heap_sm, 1, 2, 'c');
    add_transition(heap_sm, 1, 3, 'e');
    add_transition(heap_sm, 3, 0, 'r');
    
    printf("State Machine Ready. Current state: %s\n", get_current_state_name(heap_sm));
    printf("Available inputs: s(start), c(complete), e(error), r(reset), q(quit)\n");
    
    char input[MAX_INPUT_LEN];
    while (1) {
        printf("Enter input: ");
        if (!fgets(input, sizeof(input), stdin)) break;
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) break;
        
        size_t len = strlen(input
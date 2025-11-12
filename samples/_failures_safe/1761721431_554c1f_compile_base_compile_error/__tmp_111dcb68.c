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
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_state(sm, 0, "START");
    add_state(sm, 1, "READING");
    add_state(sm, 2, "PROCESSING");
    add_state(sm, 3, "DONE");
    
    add_transition(sm, 0, 1, 'r');
    add_transition(sm, 1, 2, 'p');
    add_transition(sm, 2, 3, 'd');
    add_transition(sm, 1, 0, 'b');
    add_transition(sm, 2, 1, 'b');
    
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Demo\n");
    printf("Available inputs: r (read), p (process), d (done), b (back)\n");
    printf("Current state: %s\n", get_current_state_name(sm));
    
    while (1) {
        printf("Enter input (q to quit): ");
        
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        if (input_buffer[0] == 'q' || input_buffer[0] == 'Q') {
            break;
        }
        
        if (strlen(input_buffer) < 1) {
            continue;
        }
        
        char input_char = input_buffer[0];
        
        if (process_input(sm,
//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int current_state;
    int state_count;
    int transition_count;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->state_count = 0;
    sm->transition_count = 0;
    sm->current_state = -1;
    return sm;
}

int add_state(StateMachine* sm, int id, const char* name) {
    if (sm == NULL || name == NULL) return 0;
    if (sm->state_count >= MAX_STATES) return 0;
    if (id < 0) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == id) return 0;
    }
    
    State* new_state = &sm->states[sm->state_count];
    new_state->id = id;
    strncpy(new_state->name, name, sizeof(new_state->name) - 1);
    new_state->name[sizeof(new_state->name) - 1] = '\0';
    
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine* sm, int from, int to, char input) {
    if (sm == NULL) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_exists = 1;
        if (sm->states[i].id == to) to_exists = 1;
    }
    if (!from_exists || !to_exists) return 0;
    
    Transition* new_trans = &sm->transitions[sm->transition_count];
    new_trans->from_state = from;
    new_trans->to_state = to;
    new_trans->input_char = input;
    
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine* sm, char input) {
    if (sm == NULL) return 0;
    if (sm->current_state == -1) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition* trans = &sm->transitions[i];
        if (trans->from_state == sm->current_state && trans->input_char == input) {
            sm->current_state = trans->to_state;
            return 1;
        }
    }
    return 0;
}

void print_current_state(StateMachine* sm) {
    if (sm == NULL) return;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            printf("Current state: %s (ID: %d)\n", sm->states[i].name, sm->states[i].id);
            return;
        }
    }
    printf("Invalid current state\n");
}

void destroy_state_machine(StateMachine* sm) {
    free(sm);
}

int main() {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    State states_stack[3];
    states_stack[0].id = 0;
    strcpy(states_stack[0].name, "START");
    states_stack[1].id = 1;
    strcpy(states_stack[1].name, "PROCESSING");
    states_stack[2].id = 2;
    strcpy(states_stack[2].name, "END");
    
    for (int i = 0; i < 3; i++) {
        if (!add_state(sm, states_stack[i].id, states_stack[i].name)) {
            fprintf(stderr, "Failed to add state\n");
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    if (!add_transition(sm, 0, 1, 'a') ||
        !add_transition(sm, 1, 2, 'b') ||
        !add_transition(sm, 1, 1, 'c') ||
        !add_transition(sm, 2, 0, 'r')) {
        fprintf(stderr, "Failed to add transitions\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    sm->current_state = 0;
    
    printf("State Machine Ready. Enter input characters (a,b,c,r): ");
    
    char input_buffer[MAX_INPUT_LEN];
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    for (int i = 0; input_buffer[i] != '\0' && input_buffer[i] != '\n'; i++) {
        char input
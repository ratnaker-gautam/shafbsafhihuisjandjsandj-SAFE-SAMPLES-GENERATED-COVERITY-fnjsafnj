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
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].input_char == input) {
            sm->current_state = sm->transitions[i].to_state;
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
    
    add_state(sm, 0, "START");
    add_state(sm, 1, "READING");
    add_state(sm, 2, "VALID");
    add_state(sm, 3, "ERROR");
    
    add_transition(sm, 0, 1, 'A');
    add_transition(sm, 1, 2, 'B');
    add_transition(sm, 2, 1, 'A');
    add_transition(sm, 1, 3, 'X');
    add_transition(sm, 2, 3, 'X');
    add_transition(sm, 3, 3, 'X');
    
    printf("State Machine Simulation (Enter characters: A, B, X)\n");
    printf("Enter 'q' to quit\n");
    
    char input[MAX_INPUT_LEN];
    while (1) {
        printf("Current state: %d\n", sm->current_state);
        printf("Enter input: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }
        
        for (size_t i = 0; i < strlen(input) && input[i] != '\n'; i++) {
            if (process_input(sm, input[i])) {
                printf("Transitioned with '%c'\n", input[i]);
            } else {
                printf("No transition for '%c'\n", input[i]);
            }
        }
    }
    
    destroy_state_machine(sm);
    return 0;
}
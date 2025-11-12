//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef struct {
    int from_state;
    char input_char;
    int to_state;
} Transition;

typedef struct {
    int state_id;
    int is_final;
    Transition* transitions;
    int transition_count;
} State;

typedef struct {
    State* states;
    int state_count;
    int current_state;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->states = malloc(MAX_STATES * sizeof(State));
    if (!sm->states) {
        free(sm);
        return NULL;
    }
    
    sm->state_count = 0;
    sm->current_state = 0;
    return sm;
}

int add_state(StateMachine* sm, int state_id, int is_final) {
    if (!sm || sm->state_count >= MAX_STATES || state_id < 0) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].state_id == state_id) return 0;
    }
    
    State* new_state = &sm->states[sm->state_count];
    new_state->state_id = state_id;
    new_state->is_final = is_final;
    new_state->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!new_state->transitions) return 0;
    
    new_state->transition_count = 0;
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine* sm, int from_state, char input_char, int to_state) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].state_id == from_state) {
            State* state = &sm->states[i];
            if (state->transition_count >= MAX_TRANSITIONS) return 0;
            
            Transition* trans = &state->transitions[state->transition_count];
            trans->from_state = from_state;
            trans->input_char = input_char;
            trans->to_state = to_state;
            state->transition_count++;
            return 1;
        }
    }
    return 0;
}

int process_input(StateMachine* sm, const char* input) {
    if (!sm || !input || sm->state_count == 0) return 0;
    
    sm->current_state = sm->states[0].state_id;
    
    for (int i = 0; input[i] != '\0' && i < MAX_INPUT_LEN; i++) {
        char current_char = input[i];
        int transition_found = 0;
        
        for (int j = 0; j < sm->state_count; j++) {
            if (sm->states[j].state_id == sm->current_state) {
                State* current_state = &sm->states[j];
                
                for (int k = 0; k < current_state->transition_count; k++) {
                    if (current_state->transitions[k].input_char == current_char) {
                        sm->current_state = current_state->transitions[k].to_state;
                        transition_found = 1;
                        break;
                    }
                }
                break;
            }
        }
        
        if (!transition_found) return 0;
    }
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].state_id == sm->current_state && sm->states[i].is_final) {
            return 1;
        }
    }
    return 0;
}

void free_state_machine(StateMachine* sm) {
    if (!sm) return;
    
    for (int i = 0; i < sm->state_count; i++) {
        free(sm->states[i].transitions);
    }
    free(sm->states);
    free(sm);
}

int main() {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    if (!add_state(sm, 0, 0) || !add_state(sm, 1, 1) || !add_state(sm, 2, 0)) {
        printf("Failed to add states\n");
        free_state_machine(sm);
        return 1;
    }
    
    if (!add_transition(sm, 0, 'a', 1) || !add_transition(sm, 1, 'b', 2) || 
        !add_transition(sm, 2, 'a', 1) || !add_transition(sm, 1, 'a', 0)) {
        printf("Failed to add transitions\n");
        free_state_machine(sm);
        return 1;
    }
    
    char input
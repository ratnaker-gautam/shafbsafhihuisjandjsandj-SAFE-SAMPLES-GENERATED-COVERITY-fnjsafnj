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

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

int add_state(StateMachine *sm, int id, const char *name) {
    if (!sm || id < 0 || !name || sm->state_count >= MAX_STATES) {
        return 0;
    }
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == id) {
            return 0;
        }
    }
    
    sm->states[sm->state_count].id = id;
    strncpy(sm->states[sm->state_count].name, name, 31);
    sm->states[sm->state_count].name[31] = '\0';
    
    if (sm->current_state == -1) {
        sm->current_state = id;
    }
    
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine *sm, int from, int to, char input) {
    if (!sm || from < 0 || to < 0 || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_exists = 1;
        if (sm->states[i].id == to) to_exists = 1;
    }
    
    if (!from_exists || !to_exists) {
        return 0;
    }
    
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].input_char = input;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine *sm, const char *input) {
    if (!sm || !input || sm->current_state == -1) {
        return 0;
    }
    
    size_t len = strnlen(input, MAX_INPUT_LEN);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    printf("Starting state: %d\n", sm->current_state);
    
    for (size_t i = 0; i < len; i++) {
        char current_char = input[i];
        int transition_found = 0;
        
        for (int j = 0; j < sm->transition_count; j++) {
            if (sm->transitions[j].from_state == sm->current_state && 
                sm->transitions[j].input_char == current_char) {
                int old_state = sm->current_state;
                sm->current_state = sm->transitions[j].to_state;
                printf("Input '%c': %d -> %d\n", current_char, old_state, sm->current_state);
                transition_found = 1;
                break;
            }
        }
        
        if (!transition_found) {
            printf("No transition for input '%c' in state %d\n", current_char, sm->current_state);
            return 0;
        }
    }
    
    printf("Final state: %d\n", sm->current_state);
    return 1;
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    int stack_states[3] = {0, 1, 2};
    const char *state_names[3] = {"START", "MIDDLE", "END"};
    
    for (int i = 0; i < 3; i++) {
        if (!add_state(sm, stack_states[i], state_names[i])) {
            fprintf(stderr, "Failed to add state %d\n", stack_states[i]);
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    struct {
        int from, to;
        char input;
    } stack_transitions[] = {
        {0, 1, 'a'},
        {1, 2, 'b'},
        {2, 0, 'c'},
        {1,
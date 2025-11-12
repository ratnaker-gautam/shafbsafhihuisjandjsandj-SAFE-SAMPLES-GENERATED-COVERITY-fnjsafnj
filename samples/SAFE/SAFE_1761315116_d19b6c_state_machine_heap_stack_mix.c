//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    int transition_count;
    State *accept_states;
    int accept_count;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->accept_states = malloc(MAX_STATES * sizeof(State));
    if (!sm->accept_states) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->accept_count = 0;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->accept_states);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

int add_accept_state(StateMachine *sm, State state) {
    if (!sm || sm->accept_count >= MAX_STATES) return 0;
    
    sm->accept_states[sm->accept_count] = state;
    sm->accept_count++;
    return 1;
}

int process_input(StateMachine *sm, const char *input) {
    if (!sm || !input) return 0;
    
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        int found = 0;
        
        for (int j = 0; j < sm->transition_count; j++) {
            if (sm->transitions[j].current == sm->current_state && 
                sm->transitions[j].input == c) {
                sm->current_state = sm->transitions[j].next;
                found = 1;
                break;
            }
        }
        
        if (!found) return 0;
    }
    
    for (int i = 0; i < sm->accept_count; i++) {
        if (sm->accept_states[i] == sm->current_state) {
            return 1;
        }
    }
    
    return 0;
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_E);
    add_transition(sm, STATE_C, '0', STATE_E);
    add_transition(sm, STATE_C, '1', STATE_D);
    add_transition(sm, STATE_D, '0', STATE_A);
    add_transition(sm, STATE_D, '1', STATE_B);
    add_transition(sm, STATE_E, '0', STATE_C);
    add_transition(sm, STATE_E, '1', STATE_A);
    
    add_accept_state(sm, STATE_D);
    add_accept_state(sm, STATE_E);
    
    char input[MAX_INPUT_LEN];
    printf("Enter binary string (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] != '0' && input[i] != '1') {
            fprintf(stderr, "Invalid input: only '0' and '1' allowed\n");
            destroy_state_machine(sm);
            return 1;
        }
    }
    
    int result = process_input(sm, input);
    
    if (result) {
        printf("Input accepted. Final state: %d\n", sm->current_state);
    } else {
        printf("Input rejected. Final state: %d\n", sm->current_state);
    }
    
    destroy_state_machine(sm);
    return 0;
}
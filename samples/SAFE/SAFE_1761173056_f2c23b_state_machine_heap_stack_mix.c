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
    int max_transitions;
} StateMachine;

StateMachine* create_state_machine(int max_trans) {
    if (max_trans <= 0 || max_trans > MAX_TRANSITIONS) {
        return NULL;
    }
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->transitions = malloc(sizeof(Transition) * max_trans);
    if (sm->transitions == NULL) {
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->max_transitions = max_trans;
    
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (sm == NULL || sm->transition_count >= sm->max_transitions) {
        return 0;
    }
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->input = input;
    t->next = to;
    sm->transition_count++;
    
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (sm == NULL) {
        return STATE_A;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && t->input == input) {
            sm->current_state = t->next;
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm != NULL) {
        free(sm->transitions);
        free(sm);
    }
}

const char* state_to_string(State s) {
    switch(s) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine(8);
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_A);
    add_transition(sm, STATE_C, '0', STATE_A);
    add_transition(sm, STATE_C, '1', STATE_E);
    add_transition(sm, STATE_D, '0', STATE_E);
    add_transition(sm, STATE_D, '1', STATE_C);
    
    char input_buffer[MAX_INPUT_LEN];
    
    printf("Enter binary sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    for (size_t i = 0; i < len; i++) {
        char c = input_buffer[i];
        if (c != '0' && c != '1') {
            printf("Invalid input character '%c' at position %zu\n", c, i);
            continue;
        }
        
        State new_state = process_input(sm, c);
        printf("Input '%c': %s -> %s\n", c, 
               state_to_string(sm->current_state), state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
    
    destroy_state_machine(sm);
    return 0;
}
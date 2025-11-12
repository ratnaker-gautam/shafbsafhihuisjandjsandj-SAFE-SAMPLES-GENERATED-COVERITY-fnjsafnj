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
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (sm == NULL) {
        return STATE_A;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
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

int main() {
    StateMachine *sm = create_state_machine(8);
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '3', STATE_D);
    add_transition(sm, STATE_B, '4', STATE_E);
    add_transition(sm, STATE_C, '5', STATE_D);
    add_transition(sm, STATE_C, '6', STATE_E);
    add_transition(sm, STATE_D, '7', STATE_A);
    add_transition(sm, STATE_E, '8', STATE_A);
    
    char input_buffer[MAX_INPUT_LEN];
    printf("Enter state transitions (1-8, q to quit): ");
    
    while (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if (input_buffer[0] == 'q' || input_buffer[0] == 'Q') {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len-1] == '\n') {
            input_buffer[len-1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        for (size_t i = 0; i < len; i++) {
            if (input_buffer[i] >= '1' && input_buffer[i] <= '8') {
                State new_state = process_input(sm, input_buffer[i]);
                printf("Input '%c': Moved to state %d\n", input_buffer[i], new_state);
            } else {
                printf("Invalid input '%c'\n", input_buffer[i]);
            }
        }
        
        printf("Current state: %d\n", sm->current_state);
        printf("Enter next transitions (1-8, q to quit): ");
    }
    
    destroy_state_machine(sm);
    return 0;
}
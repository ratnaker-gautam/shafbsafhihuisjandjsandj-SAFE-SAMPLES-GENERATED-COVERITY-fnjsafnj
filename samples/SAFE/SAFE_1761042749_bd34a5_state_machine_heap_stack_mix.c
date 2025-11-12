//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    NUM_STATES
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
    if (!sm) return NULL;
    
    sm->transitions = malloc(sizeof(Transition) * max_trans);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->max_transitions = max_trans;
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->transition_count >= sm->max_transitions) {
        return 0;
    }
    
    if (from >= NUM_STATES || to >= NUM_STATES) {
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
    if (!sm) return NUM_STATES;
    
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
    if (sm) {
        free(sm->transitions);
        free(sm);
    }
}

int main() {
    StateMachine *sm = create_state_machine(8);
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_A);
    add_transition(sm, STATE_C, '0', STATE_A);
    add_transition(sm, STATE_C, '1', STATE_D);
    add_transition(sm, STATE_D, '0', STATE_C);
    add_transition(sm, STATE_D, '1', STATE_B);
    
    char input_buffer[MAX_INPUT_LEN];
    printf("Enter binary sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        printf("Input error\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    printf("Initial state: A\n");
    
    for (size_t i = 0; i < len; i++) {
        char c = input_buffer[i];
        if (c != '0' && c != '1') {
            printf("Invalid input character: %c\n", c);
            continue;
        }
        
        State new_state = process_input(sm, c);
        if (new_state >= NUM_STATES) {
            printf("State machine error\n");
            break;
        }
        
        printf("Input: %c -> State: ", c);
        switch (new_state) {
            case STATE_A: printf("A\n"); break;
            case STATE_B: printf("B\n"); break;
            case STATE_C: printf("C\n"); break;
            case STATE_D: printf("D\n"); break;
            default: printf("Unknown\n"); break;
        }
    }
    
    printf("Final state: ");
    switch (sm->current_state) {
        case STATE_A: printf("A\n"); break;
        case STATE_B: printf("B\n"); break;
        case STATE_C: printf("C\n"); break;
        case STATE_D: printf("D\n"); break;
        default: printf("Unknown\n"); break;
    }
    
    destroy_state_machine(sm);
    return 0;
}
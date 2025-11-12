//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    State *stack;
    int stack_top;
    int stack_size;
} StateMachine;

StateMachine* create_state_machine(int stack_size) {
    if (stack_size <= 0 || stack_size > 1000) {
        return NULL;
    }
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->stack = malloc(sizeof(State) * stack_size);
    if (sm->stack == NULL) {
        free(sm);
        return NULL;
    }
    
    sm->stack_top = -1;
    sm->stack_size = stack_size;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm != NULL) {
        free(sm->stack);
        free(sm);
    }
}

int push_state(StateMachine *sm, State state) {
    if (sm == NULL || sm->stack_top >= sm->stack_size - 1) {
        return 0;
    }
    sm->stack_top++;
    sm->stack[sm->stack_top] = state;
    return 1;
}

int pop_state(StateMachine *sm, State *state) {
    if (sm == NULL || sm->stack_top < 0 || state == NULL) {
        return 0;
    }
    *state = sm->stack[sm->stack_top];
    sm->stack_top--;
    return 1;
}

State process_transition(State current, char input, Transition *transitions, int num_transitions) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

int main(void) {
    Transition transitions[] = {
        {STATE_A, '1', STATE_B},
        {STATE_A, '2', STATE_C},
        {STATE_B, '1', STATE_C},
        {STATE_B, '2', STATE_D},
        {STATE_C, '1', STATE_D},
        {STATE_C, '2', STATE_E},
        {STATE_D, '1', STATE_E},
        {STATE_D, '2', STATE_A},
        {STATE_E, '1', STATE_A},
        {STATE_E, '2', STATE_B}
    };
    int num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    
    StateMachine *sm = create_state_machine(50);
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    State current_state = STATE_A;
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Simulation (Enter 'q' to quit)\n");
    printf("Valid inputs: '1', '2'\n");
    printf("Initial state: A\n");
    
    while (1) {
        printf("Current state: %c | Enter input: ", 'A' + current_state);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) > 0 && input_buffer[strlen(input_buffer) - 1] == '\n') {
            input_buffer[strlen(input_buffer) - 1] = '\0';
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        if (strcmp(input_buffer, "q") == 0) {
            break;
        }
        
        if (strlen(input_buffer) != 1 || (input_buffer[0] != '1' && input_buffer[0] != '2')) {
            printf("Invalid input. Please enter '1' or '2'\n");
            continue;
        }
        
        if (!push_state(sm, current_state)) {
            printf("State stack full\n");
            break;
        }
        
        State new_state = process_transition(current_state, input_buffer[0], transitions, num_transitions);
        
        if (new_state != current_state) {
            printf("Transition: %c -> %c\n", 'A' + current_state, 'A' + new_state);
            current_state = new_state;
        } else {
            State popped_state;
            if (pop_state(sm, &popped_state)) {
                printf("No transition, popped state: %c\n", 'A' + popped_state);
            }
        }
    }
    
    printf("Final state: %c\n", 'A' + current_state);
    destroy_state_machine(sm);
    return 0;
}
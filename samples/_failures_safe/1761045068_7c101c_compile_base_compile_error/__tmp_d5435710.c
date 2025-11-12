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
    State *stack;
    int stack_top;
    int stack_size;
} StateMachine;

StateMachine* create_state_machine(int stack_size) {
    if (stack_size <= 0 || stack_size > 1000) {
        return NULL;
    }
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) {
        return NULL;
    }
    
    sm->stack = malloc(sizeof(State) * stack_size);
    if (!sm->stack) {
        free(sm);
        return NULL;
    }
    
    sm->stack_top = -1;
    sm->stack_size = stack_size;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->stack);
        free(sm);
    }
}

int push_state(StateMachine *sm, State state) {
    if (!sm || state >= NUM_STATES || sm->stack_top >= sm->stack_size - 1) {
        return 0;
    }
    sm->stack[++sm->stack_top] = state;
    return 1;
}

int pop_state(StateMachine *sm, State *state) {
    if (!sm || !state || sm->stack_top < 0) {
        return 0;
    }
    *state = sm->stack[sm->stack_top--];
    return 1;
}

State process_input(State current, char input, Transition *transitions, int num_transitions) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

int main() {
    Transition transitions[] = {
        {STATE_A, '0', STATE_B},
        {STATE_A, '1', STATE_C},
        {STATE_B, '0', STATE_A},
        {STATE_B, '1', STATE_D},
        {STATE_C, '0', STATE_D},
        {STATE_C, '1', STATE_A},
        {STATE_D, '0', STATE_C},
        {STATE_D, '1', STATE_B},
        {STATE_E, '0', STATE_A},
        {STATE_E, '1', STATE_B}
    };
    
    int num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    
    StateMachine *sm = create_state_machine(50);
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    State current_state = STATE_A;
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Simulation (Enter 'q' to quit)\n");
    printf("Current state: A\n");
    printf("Valid inputs: 0, 1\n");
    
    while (1) {
        printf("Enter input: ");
        
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        if (input_buffer[0] == 'q' && (input_buffer[1] == '\n' || input_buffer[1] == '\0')) {
            break;
        }
        
        if (strlen(input_buffer) != 2 || (input_buffer[0] != '0' && input_buffer[0] != '1') || input_buffer[1] != '\n') {
            printf("Invalid input. Please enter 0 or 1.\n");
            continue;
        }
        
        State previous_state = current_state;
        current_state = process_input(current_state, input_buffer[0], transitions, num_transitions);
        
        if (!push_state(sm, previous_state)) {
            printf("Warning: State stack full\n");
        }
        
        printf("Transition: %c -> State ", input_buffer[0]);
        switch (current_state) {
            case STATE_A: printf("A\n"); break;
            case STATE_B: printf("B\n"); break;
            case STATE_C: printf("C\n"); break;
            case STATE_D: printf("D\n"); break;
            case STATE_E: printf("E\n"); break;
            default: printf("Unknown\n"); break;
        }
        
        if (current_state == STATE_E) {
            printf("Reached terminal state E. Popping previous state...\n");
            State popped_state;
            if (pop_state(sm, &popped_state)) {
                current_state = popped_state;
                printf("Returned to state ");
                switch (current_state) {
                    case STATE_A: printf("A\n"); break;
                    case STATE_B: printf("B\n"); break;
                    case STATE_C: printf("C\n"); break;
                    case STATE_D: printf("D\n"); break;
                    case STATE_E: printf("E\n"); break;
                    default: printf("Unknown\n"); break;
                }
            }
        }
    }
    
    printf("Final state: ");
    switch (current_state) {
        case STATE_A: printf("A\n"); break;
        case STATE_B: printf("B\n"); break;
        case STATE_C: printf("C\n"); break;
        case STATE_D: printf("D\n
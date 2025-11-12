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

State process_transition(State current, char input, Transition *transitions, int num_transitions) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

void initialize_transitions(Transition *transitions, int *num_transitions) {
    transitions[0] = (Transition){STATE_A, '1', STATE_B};
    transitions[1] = (Transition){STATE_A, '2', STATE_C};
    transitions[2] = (Transition){STATE_B, '3', STATE_D};
    transitions[3] = (Transition){STATE_B, '4', STATE_E};
    transitions[4] = (Transition){STATE_C, '5', STATE_D};
    transitions[5] = (Transition){STATE_C, '6', STATE_E};
    transitions[6] = (Transition){STATE_D, '7', STATE_A};
    transitions[7] = (Transition){STATE_E, '8', STATE_A};
    *num_transitions = 8;
}

int main() {
    StateMachine *sm = create_state_machine(50);
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    Transition transitions[MAX_TRANSITIONS];
    int num_transitions;
    initialize_transitions(transitions, &num_transitions);
    
    State current_state = STATE_A;
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Simulation (Enter 'q' to quit)\n");
    printf("Valid inputs: 1-8\n");
    printf("Current state: A\n");
    
    while (1) {
        printf("Enter input: ");
        if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
            break;
        }
        
        if (input_buffer[0] == 'q' && (input_buffer[1] == '\n' || input_buffer[1] == '\0')) {
            break;
        }
        
        if (strlen(input_buffer) != 2 || input_buffer[1] != '\n') {
            printf("Invalid input format. Enter single character followed by newline.\n");
            continue;
        }
        
        char input_char = input_buffer[0];
        if (input_char < '1' || input_char > '8') {
            printf("Invalid input character. Must be 1-8.\n");
            continue;
        }
        
        if (!push_state(sm, current_state)) {
            printf("State stack overflow\n");
            break;
        }
        
        State new_state = process_transition(current_state, input_char, transitions, num_transitions);
        
        if (new_state != current_state) {
            printf("Transition: %c -> State ", input_char);
            switch (new_state) {
                case STATE_A: printf("A\n"); break;
                case STATE_B: printf("B\n"); break;
                case STATE_C: printf("C\n"); break;
                case STATE_D: printf("D\n"); break;
                case STATE_E: printf("E\n"); break;
                default: printf("Unknown\n"); break;
            }
            current_state = new_state;
        } else {
            printf("No transition for input '%c' in current state\n", input_char);
            State popped_state;
            if (pop_state(sm, &popped_state)) {
                current_state = popped_state;
                printf("Reverted to previous state\n");
            }
        }
    }
    
    printf("Final state: ");
    switch (current_state) {
        case STATE_A: printf("A\n"); break;
        case STATE_B: printf("B\n"); break;
        case STATE_C: printf("
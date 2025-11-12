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
    State* stack;
    int stack_top;
    int stack_size;
    State current_state;
    Transition* transitions;
    int transition_count;
} StateMachine;

StateMachine* create_state_machine(int stack_size) {
    if (stack_size <= 0 || stack_size > 1000) {
        return NULL;
    }
    
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->stack = malloc(sizeof(State) * stack_size);
    if (!sm->stack) {
        free(sm);
        return NULL;
    }
    
    sm->transitions = malloc(sizeof(Transition) * MAX_TRANSITIONS);
    if (!sm->transitions) {
        free(sm->stack);
        free(sm);
        return NULL;
    }
    
    sm->stack_size = stack_size;
    sm->stack_top = -1;
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) {
        free(sm->stack);
        free(sm->transitions);
        free(sm);
    }
}

int add_transition(StateMachine* sm, State from, char input, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

int push_state(StateMachine* sm, State state) {
    if (!sm || sm->stack_top >= sm->stack_size - 1) {
        return 0;
    }
    sm->stack_top++;
    sm->stack[sm->stack_top] = state;
    return 1;
}

int pop_state(StateMachine* sm) {
    if (!sm || sm->stack_top < 0) {
        return 0;
    }
    sm->current_state = sm->stack[sm->stack_top];
    sm->stack_top--;
    return 1;
}

int process_input(StateMachine* sm, char input) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            
            if (input == '[') {
                if (!push_state(sm, sm->current_state)) {
                    return 0;
                }
            } else if (input == ']') {
                if (!pop_state(sm)) {
                    return 0;
                }
                return 1;
            }
            
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void print_state(State state) {
    switch(state) {
        case STATE_A: printf("A"); break;
        case STATE_B: printf("B"); break;
        case STATE_C: printf("C"); break;
        case STATE_D: printf("D"); break;
        case STATE_E: printf("E"); break;
        default: printf("?"); break;
    }
}

int main() {
    StateMachine* sm = create_state_machine(50);
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_A);
    add_transition(sm, STATE_B, '1', STATE_D);
    add_transition(sm, STATE_C, '0', STATE_D);
    add_transition(sm, STATE_C, '1', STATE_A);
    add_transition(sm, STATE_D, '0', STATE_C);
    add_transition(sm, STATE_D, '1', STATE_B);
    add_transition(sm, STATE_A, '[', STATE_A);
    add_transition(sm, STATE_B, '[', STATE_B);
    add_transition(sm, STATE_C, '[', STATE_C);
    add_transition(sm, STATE_D, '[', STATE_D);
    
    char input[MAX_INPUT_LEN];
    printf("Enter input sequence (0,1,[,]): ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    printf("State sequence: ");
    print_state(sm->current_state);
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] != '0' && input[i] != '1' && input[i] != '[' && input[i] != ']') {
            printf("\nInvalid input character: %c\n", input[i]);
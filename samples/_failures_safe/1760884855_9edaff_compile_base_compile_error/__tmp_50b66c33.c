//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_SIZE 100

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
    int top;
    int capacity;
} StateStack;

typedef struct {
    State* heap_states;
    Transition* heap_transitions;
    int num_states;
    int num_transitions;
    StateStack stack;
} StateMachine;

StateStack create_stack(int capacity) {
    StateStack s;
    s.stack = malloc(sizeof(State) * capacity);
    if (s.stack == NULL) {
        exit(EXIT_FAILURE);
    }
    s.top = -1;
    s.capacity = capacity;
    return s;
}

void push(StateStack* s, State state) {
    if (s->top >= s->capacity - 1) {
        return;
    }
    s->stack[++(s->top)] = state;
}

State pop(StateStack* s) {
    if (s->top < 0) {
        return STATE_A;
    }
    return s->stack[(s->top)--];
}

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->num_states = 5;
    sm->num_transitions = 8;
    
    sm->heap_states = malloc(sizeof(State) * sm->num_states);
    if (sm->heap_states == NULL) {
        free(sm);
        return NULL;
    }
    
    sm->heap_transitions = malloc(sizeof(Transition) * sm->num_transitions);
    if (sm->heap_transitions == NULL) {
        free(sm->heap_states);
        free(sm);
        return NULL;
    }
    
    sm->stack = create_stack(MAX_STATES);
    
    sm->heap_states[0] = STATE_A;
    sm->heap_states[1] = STATE_B;
    sm->heap_states[2] = STATE_C;
    sm->heap_states[3] = STATE_D;
    sm->heap_states[4] = STATE_E;
    
    Transition transitions[] = {
        {STATE_A, '0', STATE_B},
        {STATE_A, '1', STATE_C},
        {STATE_B, '0', STATE_D},
        {STATE_B, '1', STATE_E},
        {STATE_C, '0', STATE_E},
        {STATE_C, '1', STATE_D},
        {STATE_D, '0', STATE_A},
        {STATE_D, '1', STATE_B}
    };
    
    for (int i = 0; i < sm->num_transitions; i++) {
        sm->heap_transitions[i] = transitions[i];
    }
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm != NULL) {
        free(sm->stack.stack);
        free(sm->heap_states);
        free(sm->heap_transitions);
        free(sm);
    }
}

State process_input(StateMachine* sm, State current, char input) {
    for (int i = 0; i < sm->num_transitions; i++) {
        if (sm->heap_transitions[i].current == current && 
            sm->heap_transitions[i].input == input) {
            return sm->heap_transitions[i].next;
        }
    }
    return current;
}

int main() {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        return EXIT_FAILURE;
    }
    
    char input_buffer[MAX_INPUT_SIZE];
    printf("Enter input sequence (0s and 1s): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        destroy_state_machine(sm);
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided.\n");
        destroy_state_machine(sm);
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_A;
    push(&sm->stack, current_state);
    
    for (size_t i = 0; i < len; i++) {
        char c = input_buffer[i];
        if (c != '0' && c != '1') {
            printf("Invalid input character: %c\n", c);
            destroy_state_machine(sm);
            return EXIT_FAILURE;
        }
        
        State next_state = process_input(sm, current_state, c);
        push(&sm->stack, next_state);
        current_state = next_state;
        
        printf("Input:
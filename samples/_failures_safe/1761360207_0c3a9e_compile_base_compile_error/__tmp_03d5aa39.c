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
    int capacity;
} StateMachine;

StateMachine* create_state_machine(State initial) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->current_state = initial;
    sm->capacity = 5;
    sm->transition_count = 0;
    sm->transitions = malloc(sm->capacity * sizeof(Transition));
    if (sm->transitions == NULL) {
        free(sm);
        return NULL;
    }
    
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (sm == NULL || sm->transitions == NULL) return 0;
    
    if (sm->transition_count >= sm->capacity) {
        int new_capacity = sm->capacity * 2;
        if (new_capacity > MAX_TRANSITIONS) new_capacity = MAX_TRANSITIONS;
        
        Transition *new_trans = realloc(sm->transitions, new_capacity * sizeof(Transition));
        if (new_trans == NULL) return 0;
        
        sm->transitions = new_trans;
        sm->capacity = new_capacity;
    }
    
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (sm == NULL || sm->transitions == NULL) return sm->current_state;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    
    return sm->current_state;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->transitions != NULL) free(sm->transitions);
    free(sm);
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
    StateMachine *sm = create_state_machine(STATE_A);
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_transition(sm, STATE_A, '0', STATE_B) ||
        !add_transition(sm, STATE_A, '1', STATE_C) ||
        !add_transition(sm, STATE_B, '0', STATE_D) ||
        !add_transition(sm, STATE_B, '1', STATE_A) ||
        !add_transition(sm, STATE_C, '0', STATE_A) ||
        !add_transition(sm, STATE_C, '1', STATE_D) ||
        !add_transition(sm, STATE_D, '0', STATE_E) ||
        !add_transition(sm, STATE_D, '1', STATE_E) ||
        !add_transition(sm, STATE_E, '0', STATE_E) ||
        !add_transition(sm, STATE_E, '1', STATE_E)) {
        fprintf(stderr, "Failed to add transitions\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    char input_buffer[MAX_INPUT_LEN];
    printf("Enter input sequence (0s and 1s, max %d chars): ", MAX_INPUT_LEN - 1);
    
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
    
    printf("State transitions: ");
    print_state(sm->current_state);
    
    for (size_t i = 0; i < len; i++) {
        if (input_buffer[i] != '0' && input_buffer[i] != '1') {
            printf("\nInvalid input character: %c\n", input_buffer[i]);
            destroy_state_machine(sm);
            return 1;
        }
        
        printf(" -> ");
        process_input(sm, input_buffer[i]);
        print_state(sm->current_state);
    }
    
    printf("\nFinal state: ");
    print_state(sm->current_state);
    printf("\n");
    
    destroy_state_machine
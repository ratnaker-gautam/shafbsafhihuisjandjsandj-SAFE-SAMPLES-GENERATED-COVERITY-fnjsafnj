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
    State from;
    State to;
    char trigger;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    size_t transition_count;
    size_t transition_capacity;
} StateMachine;

StateMachine* create_state_machine(State initial) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = initial;
    sm->transition_count = 0;
    sm->transition_capacity = 4;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_capacity);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    return sm;
}

int add_transition(StateMachine *sm, State from, State to, char trigger) {
    if (!sm || from >= NUM_STATES || to >= NUM_STATES) return 0;
    
    if (sm->transition_count >= sm->transition_capacity) {
        size_t new_cap = sm->transition_capacity * 2;
        if (new_cap > MAX_TRANSITIONS) new_cap = MAX_TRANSITIONS;
        
        Transition *new_trans = realloc(sm->transitions, sizeof(Transition) * new_cap);
        if (!new_trans) return 0;
        
        sm->transitions = new_trans;
        sm->transition_capacity = new_cap;
    }
    
    sm->transitions[sm->transition_count].from = from;
    sm->transitions[sm->transition_count].to = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return sm->current_state;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from == sm->current_state && 
            sm->transitions[i].trigger == input) {
            sm->current_state = sm->transitions[i].to;
            break;
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

const char* state_name(State s) {
    static const char* names[] = {"A", "B", "C", "D", "E"};
    if (s < NUM_STATES) return names[s];
    return "UNKNOWN";
}

int main(void) {
    StateMachine *sm = create_state_machine(STATE_A);
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, STATE_B, '1');
    add_transition(sm, STATE_A, STATE_C, '2');
    add_transition(sm, STATE_B, STATE_C, '3');
    add_transition(sm, STATE_B, STATE_D, '4');
    add_transition(sm, STATE_C, STATE_D, '5');
    add_transition(sm, STATE_C, STATE_E, '6');
    add_transition(sm, STATE_D, STATE_E, '7');
    add_transition(sm, STATE_E, STATE_A, '8');
    
    printf("State Machine Simulation\n");
    printf("Current state: %s\n", state_name(sm->current_state));
    printf("Valid inputs: 1-8, q to quit\n");
    
    char input_buffer[MAX_INPUT_LEN];
    
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
            printf("Invalid input. Must be 1-8 or q.\n");
            continue;
        }
        
        State new_state = process_input(sm, input_char);
        printf("Transitioned to state: %s\n", state_name(new_state));
    }
    
    destroy_state_machine(sm);
    printf("State machine simulation ended.\n");
    
    return 0;
}
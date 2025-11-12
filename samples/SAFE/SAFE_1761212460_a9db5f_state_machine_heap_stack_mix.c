//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    char *input_buffer;
    int buffer_size;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->buffer_size = MAX_INPUT_SIZE;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->input_buffer = malloc(MAX_INPUT_SIZE);
    if (!sm->input_buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->input_buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return sm->current_state;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    return sm->current_state;
}

void initialize_transitions(StateMachine *sm) {
    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '3', STATE_D);
    add_transition(sm, STATE_B, '4', STATE_E);
    add_transition(sm, STATE_C, '5', STATE_D);
    add_transition(sm, STATE_C, '6', STATE_E);
    add_transition(sm, STATE_D, '7', STATE_A);
    add_transition(sm, STATE_E, '8', STATE_A);
}

const char* state_name(State s) {
    static const char* names[] = {"A", "B", "C", "D", "E"};
    if (s >= 0 && s < NUM_STATES) return names[s];
    return "UNKNOWN";
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    initialize_transitions(sm);
    
    printf("State Machine initialized. Current state: %s\n", state_name(sm->current_state));
    printf("Valid inputs: 1-8. Enter 'q' to quit.\n");
    
    while (1) {
        printf("Enter input: ");
        
        if (fgets(sm->input_buffer, sm->buffer_size, stdin) == NULL) {
            break;
        }
        
        if (strlen(sm->input_buffer) == 0) continue;
        
        char input = sm->input_buffer[0];
        
        if (input == 'q' || input == 'Q') {
            break;
        }
        
        if (input < '1' || input > '8') {
            printf("Invalid input. Please enter 1-8 or 'q' to quit.\n");
            continue;
        }
        
        State new_state = process_input(sm, input);
        printf("Processed input '%c'. New state: %s\n", input, state_name(new_state));
    }
    
    printf("Final state: %s\n", state_name(sm->current_state));
    destroy_state_machine(sm);
    return 0;
}
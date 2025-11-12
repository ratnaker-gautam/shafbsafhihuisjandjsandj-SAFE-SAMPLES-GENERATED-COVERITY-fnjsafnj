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
    char *name;
} StateMachine;

StateMachine* create_state_machine(const char *name) {
    if (name == NULL || strlen(name) == 0) return NULL;
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->name = malloc(strlen(name) + 1);
    if (sm->name == NULL) {
        free(sm);
        return NULL;
    }
    strcpy(sm->name, name);
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (sm->transitions == NULL) {
        free(sm->name);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (sm == NULL) return STATE_A;
    
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
    if (sm == NULL) return;
    free(sm->name);
    free(sm->transitions);
    free(sm);
}

const char* state_to_string(State state) {
    switch(state) {
        case STATE_A: return "STATE_A";
        case STATE_B: return "STATE_B";
        case STATE_C: return "STATE_C";
        case STATE_D: return "STATE_D";
        case STATE_E: return "STATE_E";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine("SimpleParser");
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, 'a', STATE_B);
    add_transition(sm, STATE_A, 'b', STATE_C);
    add_transition(sm, STATE_B, 'c', STATE_D);
    add_transition(sm, STATE_B, 'd', STATE_E);
    add_transition(sm, STATE_C, 'e', STATE_D);
    add_transition(sm, STATE_C, 'f', STATE_E);
    add_transition(sm, STATE_D, 'g', STATE_A);
    add_transition(sm, STATE_E, 'h', STATE_A);
    
    char input_buffer[MAX_INPUT_SIZE];
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    
    for (size_t i = 0; i < len; i++) {
        State new_state = process_input(sm, input_buffer[i]);
        printf("Input '%c': %s -> %s\n", 
               input_buffer[i], 
               state_to_string(sm->current_state), 
               state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
    
    destroy_state_machine(sm);
    return 0;
}
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
    int max_transitions;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_A;
    sm->max_transitions = MAX_TRANSITIONS;
    sm->transition_count = 0;
    
    sm->transitions = malloc(sizeof(Transition) * sm->max_transitions);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    return sm;
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || !sm->transitions) return 0;
    if (sm->transition_count >= sm->max_transitions) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm || !sm->transitions) return sm ? sm->current_state : STATE_A;
    
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
    if (sm) {
        if (sm->transitions) {
            free(sm->transitions);
        }
        free(sm);
    }
}

void initialize_transitions(StateMachine *sm) {
    if (!sm) return;
    
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_A);
    add_transition(sm, STATE_C, '0', STATE_A);
    add_transition(sm, STATE_C, '1', STATE_E);
    add_transition(sm, STATE_D, '0', STATE_E);
    add_transition(sm, STATE_D, '1', STATE_B);
    add_transition(sm, STATE_E, '0', STATE_C);
    add_transition(sm, STATE_E, '1', STATE_D);
}

const char* state_to_string(State s) {
    static const char* names[] = {"A", "B", "C", "D", "E"};
    if (s >= 0 && s < NUM_STATES) {
        return names[s];
    }
    return "UNKNOWN";
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    initialize_transitions(sm);
    
    char input_buffer[MAX_INPUT_SIZE];
    printf("Enter binary sequence (0s and 1s only, max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    printf("Processing sequence: %s\n", input_buffer);
    
    for (size_t i = 0; i < len; i++) {
        char current_char = input_buffer[i];
        
        if (current_char != '0' && current_char != '1') {
            printf("Invalid input character: '%c' at position %zu\n", current_char, i + 1);
            destroy_state_machine(sm);
            return 1;
        }
        
        State new_state = process_input(sm, current_char);
        printf("Input '%c' -> State: %s\n", current_char, state_to_string(new_state));
    }
    
    printf("Final state: %s\n", state_to_string(sm->current_state));
    
    destroy_state_machine(sm);
    return 0;
}
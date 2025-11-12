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
    State current_state;
    Transition *transitions;
    size_t transition_count;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_A;
    sm->transition_count = 5;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->transitions[0] = (Transition){STATE_A, '0', STATE_B};
    sm->transitions[1] = (Transition){STATE_A, '1', STATE_C};
    sm->transitions[2] = (Transition){STATE_B, '0', STATE_D};
    sm->transitions[3] = (Transition){STATE_B, '1', STATE_E};
    sm->transitions[4] = (Transition){STATE_C, '0', STATE_E};
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm);
    }
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return sm->current_state;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    return sm->current_state;
}

const char* state_to_string(State state) {
    static const char *state_names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
    };
    
    if (state < NUM_STATES) {
        return state_names[state];
    }
    return "UNKNOWN_STATE";
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Simulation\n");
    printf("Valid inputs: '0' or '1'\n");
    printf("Enter input sequence (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t input_len = strnlen(input_buffer, sizeof(input_buffer));
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    printf("\nInitial state: %s\n", state_to_string(sm->current_state));
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = input_buffer[i];
        
        if (current_char != '0' && current_char != '1') {
            printf("Invalid input '%c' at position %zu\n", current_char, i + 1);
            continue;
        }
        
        State new_state = process_input(sm, current_char);
        printf("Input '%c': %s -> %s\n", 
               current_char, 
               state_to_string(sm->current_state), 
               state_to_string(new_state));
    }
    
    printf("\nFinal state: %s\n", state_to_string(sm->current_state));
    
    destroy_state_machine(sm);
    return 0;
}
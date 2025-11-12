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
    STATE_ERROR
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State *states;
    Transition *transitions;
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->states = malloc(MAX_STATES * sizeof(State));
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->states || !sm->transitions) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->state_count = 5;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    
    sm->transition_count = 8;
    sm->transitions[0] = (Transition){STATE_A, '0', STATE_B};
    sm->transitions[1] = (Transition){STATE_A, '1', STATE_C};
    sm->transitions[2] = (Transition){STATE_B, '0', STATE_D};
    sm->transitions[3] = (Transition){STATE_B, '1', STATE_E};
    sm->transitions[4] = (Transition){STATE_C, '0', STATE_E};
    sm->transitions[5] = (Transition){STATE_C, '1', STATE_D};
    sm->transitions[6] = (Transition){STATE_D, '0', STATE_A};
    sm->transitions[7] = (Transition){STATE_E, '1', STATE_A};
    
    sm->current_state = STATE_A;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    
    sm->current_state = STATE_ERROR;
    return STATE_ERROR;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Simulation (Valid inputs: 0, 1)\n");
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    printf("Enter input sequence (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    printf("\nProcessing input sequence: %s\n", input_buffer);
    printf("State transitions:\n");
    printf("Start -> %s\n", state_to_string(sm->current_state));
    
    for (size_t i = 0; i < len; i++) {
        char current_char = input_buffer[i];
        
        if (current_char != '0' && current_char != '1') {
            printf("Invalid input '%c' at position %zu\n", current_char, i + 1);
            destroy_state_machine(sm);
            return 1;
        }
        
        State new_state = process_input(sm, current_char);
        printf("Input '%c' -> %s\n", current_char, state_to_string(new_state));
        
        if (new_state == STATE_ERROR) {
            printf("Invalid transition detected\n");
            break;
        }
    }
    
    printf("\nFinal state: %s\n", state_to_string(sm->current_state));
    
    destroy_state_machine(sm);
    return 0;
}
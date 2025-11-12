//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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
    STATE_E
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

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->state_count = 5;
    sm->states = malloc(sizeof(State) * sm->state_count);
    if (!sm->states) {
        free(sm);
        return NULL;
    }
    
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    
    sm->transition_count = 8;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (!sm->transitions) {
        free(sm->states);
        free(sm);
        return NULL;
    }
    
    sm->transitions[0] = (Transition){STATE_A, '0', STATE_B};
    sm->transitions[1] = (Transition){STATE_A, '1', STATE_C};
    sm->transitions[2] = (Transition){STATE_B, '0', STATE_D};
    sm->transitions[3] = (Transition){STATE_B, '1', STATE_A};
    sm->transitions[4] = (Transition){STATE_C, '0', STATE_A};
    sm->transitions[5] = (Transition){STATE_C, '1', STATE_E};
    sm->transitions[6] = (Transition){STATE_D, '0', STATE_E};
    sm->transitions[7] = (Transition){STATE_D, '1', STATE_B};
    
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

void print_state(State state) {
    const char *state_names[] = {"A", "B", "C", "D", "E"};
    if (state >= STATE_A && state <= STATE_E) {
        printf("State %s\n", state_names[state]);
    }
}

int validate_input_string(const char *input) {
    if (!input) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Finite State Machine Simulation\n");
    printf("Valid inputs: '0' or '1'\n");
    printf("Enter input sequence (max %d characters): ", MAX_INPUT_LEN);
    
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input_string(input_buffer)) {
        fprintf(stderr, "Invalid input sequence\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("\nInitial state: ");
    print_state(sm->current_state);
    
    printf("\nProcessing sequence: %s\n", input_buffer);
    
    for (size_t i = 0; input_buffer[i] != '\0'; i++) {
        State new_state = process_input(sm, input_buffer[i]);
        printf("Input '%c' -> ", input_buffer[i]);
        print_state(new_state);
    }
    
    printf("\nFinal state: ");
    print_state(sm->current_state);
    
    destroy_state_machine(sm);
    return 0;
}
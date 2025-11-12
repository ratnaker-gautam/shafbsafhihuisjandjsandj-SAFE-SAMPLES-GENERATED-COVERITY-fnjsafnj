//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_SIZE 64

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
    char *input_buffer;
    size_t input_size;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
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
    
    sm->current_state = STATE_A;
    sm->transition_count = 0;
    sm->input_size = 0;
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
    if (!sm) return NUM_STATES;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    return sm->current_state;
}

void initialize_transitions(StateMachine *sm) {
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_A);
    add_transition(sm, STATE_C, '0', STATE_A);
    add_transition(sm, STATE_C, '1', STATE_D);
    add_transition(sm, STATE_D, '0', STATE_C);
    add_transition(sm, STATE_D, '1', STATE_B);
    add_transition(sm, STATE_E, '0', STATE_A);
    add_transition(sm, STATE_E, '1', STATE_B);
}

const char* state_name(State s) {
    static const char* names[] = {"A", "B", "C", "D", "E", "INVALID"};
    if (s < NUM_STATES) return names[s];
    return names[NUM_STATES];
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    initialize_transitions(sm);
    
    printf("State Machine initialized. Current state: %s\n", state_name(sm->current_state));
    printf("Enter input sequence (0s and 1s, max %d chars): ", MAX_INPUT_SIZE - 1);
    
    if (fgets(sm->input_buffer, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    sm->input_size = strlen(sm->input_buffer);
    if (sm->input_size > 0 && sm->input_buffer[sm->input_size - 1] == '\n') {
        sm->input_buffer[sm->input_size - 1] = '\0';
        sm->input_size--;
    }
    
    printf("Processing input: %s\n", sm->input_buffer);
    printf("State transitions:\n");
    printf("Start: %s\n", state_name(sm->current_state));
    
    for (size_t i = 0; i < sm->input_size; i++) {
        char input_char = sm->input_buffer[i];
        if (input_char != '0' && input_char != '1') {
            printf("Invalid input character: '%c'\n", input_char);
            continue;
        }
        
        State previous = sm->current_state;
        State new_state = process_input(sm, input_char);
        
        if (new_state != NUM_STATES) {
            printf("Input '%c': %s -> %s\n", input_char, state_name(previous), state_name(new_state));
        } else {
            printf("Invalid transition from state %s with input '%c'\n", state_name(previous), input_char);
        }
    }
    
    printf("Final state: %s\n", state_name(sm->current_state));
    
    destroy_state_machine(sm);
    return 0
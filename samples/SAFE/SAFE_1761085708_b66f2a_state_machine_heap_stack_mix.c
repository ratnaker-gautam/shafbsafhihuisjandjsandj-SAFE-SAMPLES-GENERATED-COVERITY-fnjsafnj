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
    if (sm == NULL) return NUM_STATES;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
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
    static const char *state_names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E", "INVALID"
    };
    
    if (state < 0 || state >= NUM_STATES) return state_names[NUM_STATES];
    return state_names[state];
}

int main(void) {
    StateMachine *sm = create_state_machine("DemoStateMachine");
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '1', STATE_B);
    add_transition(sm, STATE_A, '2', STATE_C);
    add_transition(sm, STATE_B, '3', STATE_D);
    add_transition(sm, STATE_B, '4', STATE_E);
    add_transition(sm, STATE_C, '5', STATE_D);
    add_transition(sm, STATE_C, '6', STATE_E);
    add_transition(sm, STATE_D, '7', STATE_A);
    add_transition(sm, STATE_E, '8', STATE_A);
    
    printf("State Machine: %s\n", sm->name);
    printf("Initial state: %s\n", state_to_string(sm->current_state));
    printf("Enter input characters (1-8) or 'q' to quit:\n");
    
    char input_buffer[MAX_INPUT_LEN];
    while (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
        if (input_buffer[0] == 'q' && (input_buffer[1] == '\n' || input_buffer[1] == '\0')) {
            break;
        }
        
        for (size_t i = 0; i < strlen(input_buffer); i++) {
            if (input_buffer[i] == '\n') continue;
            
            State new_state = process_input(sm, input_buffer[i]);
            if (new_state == NUM_STATES) {
                printf("Invalid state transition\n");
                break;
            }
            
            printf("Input '%c': %s -> %s\n", 
                   input_buffer[i], 
                   state_to_string(sm->current_state), 
                   state_to_string(new_state));
        }
        
        printf("Current state: %s\n", state_to_string(sm->current_state));
        printf("Enter next input or 'q' to quit: ");
    }
    
    destroy_state_machine(sm);
    return 0;
}
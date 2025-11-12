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
    if (from >= NUM_STATES || to >= NUM_STATES) return 0;
    
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

const char* state_to_string(State state) {
    static const char *state_names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
    };
    
    if (state < NUM_STATES) {
        return state_names[state];
    }
    return "UNKNOWN";
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    int stack_transitions[MAX_TRANSITIONS];
    int stack_count = 0;
    
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
    
    printf("State Machine initialized. Current state: %s\n", state_to_string(sm->current_state));
    printf("Enter input sequence (0s and 1s, max %d characters): ", MAX_INPUT_SIZE);
    
    char input_buffer[MAX_INPUT_SIZE + 1];
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Error reading input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    printf("Processing input: %s\n", input_buffer);
    
    for (size_t i = 0; i < input_len; i++) {
        char current_char = input_buffer[i];
        
        if (current_char != '0' && current_char != '1') {
            printf("Invalid input character '%c' at position %zu. Only 0 and 1 are allowed.\n", 
                   current_char, i + 1);
            destroy_state_machine(sm);
            return 1;
        }
        
        State previous_state = sm->current_state;
        State new_state = process_input(sm, current_char);
        
        if (stack_count < MAX_TRANSITIONS) {
            stack_transitions[stack_count++] = (int)previous_state;
        }
        
        printf("Input '%c': %s -> %s\n", current_char, 
               state_to_string(previous_state), state_to_string(new_state));
    }
    
    printf("Final state:
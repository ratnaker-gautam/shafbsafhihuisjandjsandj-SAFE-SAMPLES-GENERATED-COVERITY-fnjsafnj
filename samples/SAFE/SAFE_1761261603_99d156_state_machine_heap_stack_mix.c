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
    
    Transition temp_transitions[] = {
        {STATE_A, '0', STATE_B},
        {STATE_A, '1', STATE_C},
        {STATE_B, '0', STATE_D},
        {STATE_B, '1', STATE_E},
        {STATE_C, '0', STATE_E}
    };
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        sm->transitions[i] = temp_transitions[i];
    }
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm);
    }
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return NUM_STATES;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    
    return NUM_STATES;
}

void print_state_name(State state) {
    const char *names[] = {"STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E", "INVALID"};
    if (state < NUM_STATES) {
        printf("%s", names[state]);
    } else {
        printf("%s", names[NUM_STATES]);
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input_buffer[MAX_INPUT_LEN];
    printf("Enter input sequence (0s and 1s, max %d chars): ", MAX_INPUT_LEN - 1);
    
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
    
    printf("Initial state: ");
    print_state_name(sm->current_state);
    printf("\n");
    
    for (size_t i = 0; i < len; i++) {
        char current_char = input_buffer[i];
        if (current_char != '0' && current_char != '1') {
            printf("Invalid input '%c' at position %zu\n", current_char, i);
            continue;
        }
        
        State new_state = process_input(sm, current_char);
        if (new_state == NUM_STATES) {
            printf("No transition for state ");
            print_state_name(sm->current_state);
            printf(" with input '%c'\n", current_char);
        } else {
            printf("Input '%c': ", current_char);
            print_state_name(new_state);
            printf("\n");
        }
    }
    
    printf("Final state: ");
    print_state_name(sm->current_state);
    printf("\n");
    
    destroy_state_machine(sm);
    return 0;
}
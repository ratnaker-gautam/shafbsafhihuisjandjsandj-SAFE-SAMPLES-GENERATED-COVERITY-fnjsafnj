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
    int num_transitions;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_A;
    sm->num_transitions = 0;
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, char input, State to) {
    if (!sm || sm->num_transitions >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->num_transitions].current = from;
    sm->transitions[sm->num_transitions].input = input;
    sm->transitions[sm->num_transitions].next = to;
    sm->num_transitions++;
    return 1;
}

State process_input(StateMachine *sm, char input) {
    if (!sm) return sm ? sm->current_state : STATE_A;
    
    for (int i = 0; i < sm->num_transitions; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    return sm->current_state;
}

void print_state_name(State state) {
    const char *names[] = {"A", "B", "C", "D", "E"};
    if (state >= 0 && state < NUM_STATES) {
        printf("%s", names[state]);
    } else {
        printf("UNKNOWN");
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_A, '0', STATE_B);
    add_transition(sm, STATE_A, '1', STATE_C);
    add_transition(sm, STATE_B, '0', STATE_D);
    add_transition(sm, STATE_B, '1', STATE_A);
    add_transition(sm, STATE_C, '0', STATE_A);
    add_transition(sm, STATE_C, '1', STATE_D);
    add_transition(sm, STATE_D, '0', STATE_E);
    add_transition(sm, STATE_D, '1', STATE_E);
    add_transition(sm, STATE_E, '0', STATE_E);
    add_transition(sm, STATE_E, '1', STATE_E);
    
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter binary sequence (max %d chars): ", MAX_INPUT_LEN);
    if (!fgets(input_buffer, sizeof(input_buffer), stdin)) {
        fprintf(stderr, "Input error\n");
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
    
    printf("Processing: ");
    for (size_t i = 0; i < len; i++) {
        char c = input_buffer[i];
        if (c != '0' && c != '1') {
            printf("\nInvalid input character: %c\n", c);
            destroy_state_machine(sm);
            return 1;
        }
        
        State new_state = process_input(sm, c);
        printf("%c->", c);
        print_state_name(new_state);
        if (i < len - 1) printf(" ");
    }
    printf("\n");
    
    printf("Final state: ");
    print_state_name(sm->current_state);
    printf("\n");
    
    destroy_state_machine(sm);
    return 0;
}
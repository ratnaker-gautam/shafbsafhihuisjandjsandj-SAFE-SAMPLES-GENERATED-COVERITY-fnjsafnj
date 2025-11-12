//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef struct {
    int id;
    char name[32];
} State;

typedef struct {
    int from_state;
    int to_state;
    char input_char;
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int current_state;
    int state_count;
    int transition_count;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    sm->state_count = 0;
    sm->transition_count = 0;
    sm->current_state = 0;
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    free(sm);
}

int add_state(StateMachine* sm, int id, const char* name) {
    if (sm == NULL || sm->state_count >= MAX_STATES) {
        return 0;
    }
    if (strlen(name) >= 32) {
        return 0;
    }
    sm->states[sm->state_count].id = id;
    strcpy(sm->states[sm->state_count].name, name);
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine* sm, int from, int to, char input) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].input_char = input;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine* sm, char input) {
    if (sm == NULL) {
        return 0;
    }
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].input_char == input) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

const char* get_current_state_name(StateMachine* sm) {
    if (sm == NULL) {
        return "INVALID";
    }
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            return sm->states[i].name;
        }
    }
    return "UNKNOWN";
}

int main() {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        printf("Failed to create state machine\n");
        return 1;
    }

    State states_stack[3];
    states_stack[0].id = 0;
    strcpy(states_stack[0].name, "START");
    states_stack[1].id = 1;
    strcpy(states_stack[1].name, "PROCESSING");
    states_stack[2].id = 2;
    strcpy(states_stack[2].name, "END");

    for (int i = 0; i < 3; i++) {
        if (!add_state(sm, states_stack[i].id, states_stack[i].name)) {
            printf("Failed to add state\n");
            destroy_state_machine(sm);
            return 1;
        }
    }

    Transition* transitions_heap = malloc(4 * sizeof(Transition));
    if (transitions_heap == NULL) {
        printf("Failed to allocate transitions\n");
        destroy_state_machine(sm);
        return 1;
    }

    transitions_heap[0].from_state = 0;
    transitions_heap[0].to_state = 1;
    transitions_heap[0].input_char = 'a';
    transitions_heap[1].from_state = 1;
    transitions_heap[1].to_state = 2;
    transitions_heap[1].input_char = 'b';
    transitions_heap[2].from_state = 1;
    transitions_heap[2].to_state = 0;
    transitions_heap[2].input_char = 'r';
    transitions_heap[3].from_state = 2;
    transitions_heap[3].to_state = 0;
    transitions_heap[3].input_char = 'r';

    for (int i = 0; i < 4; i++) {
        if (!add_transition(sm, transitions_heap[i].from_state, 
                           transitions_heap[i].to_state, 
                           transitions_heap[i].input_char)) {
            printf("Failed to add transition\n");
            free(transitions_heap);
            destroy_state_machine(sm);
            return 1;
        }
    }

    free(transitions_heap);

    char input_buffer[MAX_INPUT_LEN];
    printf("State Machine Simulation (enter 'q' to quit)\n");
    printf("Valid inputs: a, b, r\n");
    printf("Current state: %s\n", get_current_state_name(sm));

    while (1) {
        printf("Enter input: ");
        if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
            break;
        }

        if (strlen(input_buffer) == 0) {
            continue;
        }

        char input_char = input_buffer[0];
        if (input_char
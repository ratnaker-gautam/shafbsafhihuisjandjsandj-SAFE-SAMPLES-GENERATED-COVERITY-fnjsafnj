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
    char trigger;
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
    sm->current_state = -1;
    return sm;
}

int add_state(StateMachine* sm, int id, const char* name) {
    if (sm == NULL || sm->state_count >= MAX_STATES) {
        return 0;
    }
    if (name == NULL || strlen(name) >= 32) {
        return 0;
    }
    sm->states[sm->state_count].id = id;
    strncpy(sm->states[sm->state_count].name, name, 31);
    sm->states[sm->state_count].name[31] = '\0';
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine* sm, int from, int to, char trigger) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine* sm, char input) {
    if (sm == NULL || sm->current_state == -1) {
        return 0;
    }
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].trigger == input) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

void print_current_state(StateMachine* sm) {
    if (sm == NULL || sm->current_state == -1) {
        printf("No current state\n");
        return;
    }
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            printf("Current state: %s (ID: %d)\n", sm->states[i].name, sm->states[i].id);
            return;
        }
    }
    printf("Invalid current state\n");
}

void cleanup_state_machine(StateMachine* sm) {
    free(sm);
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
            cleanup_state_machine(sm);
            return 1;
        }
    }

    Transition transitions_stack[4];
    transitions_stack[0].from_state = 0;
    transitions_stack[0].to_state = 1;
    transitions_stack[0].trigger = 's';
    transitions_stack[1].from_state = 1;
    transitions_stack[1].to_state = 2;
    transitions_stack[1].trigger = 'e';
    transitions_stack[2].from_state = 1;
    transitions_stack[2].to_state = 0;
    transitions_stack[2].trigger = 'r';
    transitions_stack[3].from_state = 2;
    transitions_stack[3].to_state = 0;
    transitions_stack[3].trigger = 'r';

    for (int i = 0; i < 4; i++) {
        if (!add_transition(sm, transitions_stack[i].from_state, 
                           transitions_stack[i].to_state, 
                           transitions_stack[i].trigger)) {
            printf("Failed to add transition\n");
            cleanup_state_machine(sm);
            return 1;
        }
    }

    sm->current_state = 0;

    printf("State Machine Ready. Commands: s=start, e=end, r=reset, q=quit\n");
    
    char input_buffer[MAX_INPUT_LEN];
    while (1) {
        print_current_state(sm);
        printf("Enter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        char command = input_buffer[0];
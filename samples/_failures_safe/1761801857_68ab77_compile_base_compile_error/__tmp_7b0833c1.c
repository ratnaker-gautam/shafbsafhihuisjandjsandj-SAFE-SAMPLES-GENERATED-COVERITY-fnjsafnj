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
    if (sm == NULL) return NULL;
    
    sm->state_count = 0;
    sm->transition_count = 0;
    sm->current_state = 0;
    
    return sm;
}

int add_state(StateMachine* sm, int id, const char* name) {
    if (sm == NULL || name == NULL) return 0;
    if (sm->state_count >= MAX_STATES) return 0;
    if (id < 0) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == id) return 0;
    }
    
    sm->states[sm->state_count].id = id;
    strncpy(sm->states[sm->state_count].name, name, 31);
    sm->states[sm->state_count].name[31] = '\0';
    sm->state_count++;
    
    return 1;
}

int add_transition(StateMachine* sm, int from, int to, char input) {
    if (sm == NULL) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    int from_found = 0, to_found = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_found = 1;
        if (sm->states[i].id == to) to_found = 1;
    }
    
    if (!from_found || !to_found) return 0;
    
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].input_char = input;
    sm->transition_count++;
    
    return 1;
}

int process_input(StateMachine* sm, char input) {
    if (sm == NULL) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].input_char == input) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    
    return 0;
}

void print_current_state(StateMachine* sm) {
    if (sm == NULL) return;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            printf("Current state: %s (ID: %d)\n", sm->states[i].name, sm->states[i].id);
            return;
        }
    }
}

void cleanup_state_machine(StateMachine* sm) {
    free(sm);
}

int main() {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_state(sm, 0, "START")) {
        fprintf(stderr, "Failed to add state\n");
        cleanup_state_machine(sm);
        return 1;
    }
    if (!add_state(sm, 1, "PROCESSING")) {
        fprintf(stderr, "Failed to add state\n");
        cleanup_state_machine(sm);
        return 1;
    }
    if (!add_state(sm, 2, "COMPLETE")) {
        fprintf(stderr, "Failed to add state\n");
        cleanup_state_machine(sm);
        return 1;
    }
    
    if (!add_transition(sm, 0, 1, 's')) {
        fprintf(stderr, "Failed to add transition\n");
        cleanup_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, 1, 2, 'c')) {
        fprintf(stderr, "Failed to add transition\n");
        cleanup_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, 1, 0, 'r')) {
        fprintf(stderr, "Failed to add transition\n");
        cleanup_state_machine(sm);
        return 1;
    }
    
    char input_buffer[MAX_INPUT_LEN];
    printf("State Machine Simulation\n");
    printf("Available inputs: s (start), c (complete), r (reset), q (quit)\n");
    
    while (1) {
        print_current_state(sm);
        printf("Enter input: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) == 0) continue;
        
        char input_char = input_buffer[0];
        
        if (input_char ==
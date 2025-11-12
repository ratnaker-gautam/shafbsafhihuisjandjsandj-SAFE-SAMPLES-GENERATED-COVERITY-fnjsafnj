//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_LEN 100

typedef struct {
    int from_state;
    int to_state;
    char trigger;
} Transition;

typedef struct {
    int id;
    char name[20];
} State;

typedef struct {
    State *states;
    Transition *transitions;
    int current_state;
    int state_count;
    int transition_count;
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
    
    sm->state_count = 0;
    sm->transition_count = 0;
    sm->current_state = -1;
    
    return sm;
}

int add_state(StateMachine *sm, int id, const char *name) {
    if (!sm || id < 0 || id >= MAX_STATES || !name) return 0;
    if (sm->state_count >= MAX_STATES) return 0;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == id) return 0;
    }
    
    sm->states[sm->state_count].id = id;
    strncpy(sm->states[sm->state_count].name, name, 19);
    sm->states[sm->state_count].name[19] = '\0';
    
    if (sm->current_state == -1) {
        sm->current_state = id;
    }
    
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine *sm, int from, int to, char trigger) {
    if (!sm || from < 0 || to < 0) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_exists = 1;
        if (sm->states[i].id == to) to_exists = 1;
    }
    
    if (!from_exists || !to_exists) return 0;
    
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    
    return 1;
}

int process_input(StateMachine *sm, char input) {
    if (!sm) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].trigger == input) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

void print_current_state(StateMachine *sm) {
    if (!sm) return;
    
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            printf("Current state: %s (ID: %d)\n", sm->states[i].name, sm->states[i].id);
            return;
        }
    }
}

void destroy_state_machine(StateMachine *sm) {
    if (!sm) return;
    free(sm->states);
    free(sm->transitions);
    free(sm);
}

int main() {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        printf("Failed to create state machine\n");
        return 1;
    }
    
    add_state(sm, 0, "START");
    add_state(sm, 1, "PROCESSING");
    add_state(sm, 2, "COMPLETE");
    add_state(sm, 3, "ERROR");
    
    add_transition(sm, 0, 1, 's');
    add_transition(sm, 1, 2, 'c');
    add_transition(sm, 1, 3, 'e');
    add_transition(sm, 3, 0, 'r');
    add_transition(sm, 2, 0, 'r');
    
    printf("State Machine Demo\n");
    printf("Available inputs: s (start), c (complete), e (error), r (reset), q (quit)\n");
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    while (running) {
        print_current_state(sm);
        printf("Enter input: ");
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        if (strlen(input) == 0) continue;
        
        char cmd = input[0];
        
        if (cmd == 'q') {
            running = 0;
        } else {
            if (process_input(sm, cmd)) {
                printf("Transition successful\n");
            } else {
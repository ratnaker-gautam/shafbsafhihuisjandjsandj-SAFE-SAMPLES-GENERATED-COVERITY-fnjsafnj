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
    if (sm == NULL || sm->state_count >= MAX_STATES || name == NULL) {
        return 0;
    }
    if (strlen(name) >= 32) {
        return 0;
    }
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == id) {
            return 0;
        }
    }
    sm->states[sm->state_count].id = id;
    strncpy(sm->states[sm->state_count].name, name, 31);
    sm->states[sm->state_count].name[31] = '\0';
    sm->state_count++;
    if (sm->current_state == -1) {
        sm->current_state = id;
    }
    return 1;
}

int add_transition(StateMachine* sm, int from, int to, char trigger) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    int from_found = 0, to_found = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_found = 1;
        if (sm->states[i].id == to) to_found = 1;
    }
    if (!from_found || !to_found) {
        return 0;
    }
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine* sm, char input) {
    if (sm == NULL) {
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
    if (sm == NULL) {
        return;
    }
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state) {
            printf("Current state: %s (ID: %d)\n", sm->states[i].name, sm->current_state);
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

    State states_stack[4] = {
        {0, "START"},
        {1, "PROCESSING"},
        {2, "PAUSED"},
        {3, "FINISHED"}
    };

    for (int i = 0; i < 4; i++) {
        if (!add_state(sm, states_stack[i].id, states_stack[i].name)) {
            fprintf(stderr, "Failed to add state %d\n", states_stack[i].id);
            cleanup_state_machine(sm);
            return 1;
        }
    }

    Transition transitions_stack[5] = {
        {0, 1, 's'},
        {1, 2, 'p'},
        {1, 3, 'f'},
        {2, 1, 'r'},
        {2, 3, 'f'}
    };

    for (int i = 0; i < 5; i++) {
        if (!add_transition(sm, transitions_stack[i].from_state, 
                           transitions_stack[i].to_state, 
                           transitions_stack[i].trigger)) {
            fprintf(stderr, "Failed to add transition %d\n", i);
            cleanup_state_machine(sm);
            return 1;
        }
    }

    printf("State Machine initialized. Available triggers:\n");
    printf("s - start processing\n");
    printf("p - pause processing\n");
    printf("r - resume processing\n");
    printf("f - finish\n");
    printf("q - quit\n\n");

    char input[MAX_INPUT_LEN];
    while (1) {
        print_current_state(sm);
        printf("Enter trigger: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL
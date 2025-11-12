//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_INPUT_SIZE 100

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

void destroy_state_machine(StateMachine* sm) {
    free(sm);
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
    if (sm->current_state == -1) {
        sm->current_state = id;
    }
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
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }

    add_state(sm, 0, "START");
    add_state(sm, 1, "PROCESSING");
    add_state(sm, 2, "VALIDATING");
    add_state(sm, 3, "COMPLETE");
    add_state(sm, 4, "ERROR");

    add_transition(sm, 0, 1, 's');
    add_transition(sm, 1, 2, 'p');
    add_transition(sm, 2, 3, 'v');
    add_transition(sm, 2, 4, 'e');
    add_transition(sm, 1, 4, 'e');
    add_transition(sm, 3, 0, 'r');
    add_transition(sm, 4, 0, 'r');

    printf("State Machine Ready. Current state: %s\n", get_current_state_name(sm));
    printf("Available triggers: s(start), p(process), v(validate), e(error), r(reset)\n");
    printf("Enter sequence of triggers (max %d characters): ", MAX_INPUT_SIZE);

    char input[MAX_INPUT_SIZE];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }

    for (int i = 0; i < MAX_INPUT_SIZE && input[i] != '\0' && input[i] != '\n'; i++) {
        char trigger = input[i];
        if (process_input(sm, trigger)) {
            printf("Trigger '%c': Transitioned to %s\n", trigger, get_current_state_name(sm));
        } else {
            printf("Trigger '%c': No valid transition from current state\n", trigger);
        }
    }

    printf("Final state: %s\n", get_current_state_name(sm));
    destroy_state_machine(sm);
    return 0;
}
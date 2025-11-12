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

StateMachine* create_state_machine(void) {
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

void destroy_state_machine(StateMachine* sm) {
    free(sm);
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }

    if (!add_state(sm, 0, "START") || 
        !add_state(sm, 1, "READING") || 
        !add_state(sm, 2, "PROCESSING") || 
        !add_state(sm, 3, "DONE")) {
        fprintf(stderr, "Failed to add states\n");
        destroy_state_machine(sm);
        return 1;
    }

    if (!add_transition(sm, 0, 1, 'B') || 
        !add_transition(sm, 1, 2, 'P') || 
        !add_transition(sm, 2, 3, 'F') || 
        !add_transition(sm, 1, 1, 'C') || 
        !add_transition(sm, 2, 2, 'R')) {
        fprintf(stderr, "Failed to add transitions\n");
        destroy_state_machine(sm);
        return 1;
    }

    sm->current_state = 0;

    printf("State Machine Ready. Commands: B(begin), C(continue), P(process), R(retry), F(finish)\n");
    printf("Current state: %s\n", sm->states[0].name);

    char input_buffer[MAX_INPUT_SIZE];
    while (sm->current_state != 3) {
        printf("Enter command: ");
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
            continue;
        }
        char command = input_buffer[0];
        if (process_input(sm, command)) {
            for (int i = 0; i < sm->state_count; i++) {
                if (sm->states[i].id == sm->current_state) {
                    printf("Transitioned to state: %s\n", sm->states[i].name);
                    break;
                }
            }
        } else {
            printf("Invalid transition from current state\n");
        }
    }

    printf("State machine completed successfully\n");
    destroy_state_machine(sm);
    return 0;
}
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
    sm->current_state = -1;
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    free(sm);
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

int add_transition(StateMachine* sm, int from, int to, char input) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    int from_exists = 0, to_exists = 0;
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == from) from_exists = 1;
        if (sm->states[i].id == to) to_exists = 1;
    }
    if (!from_exists || !to_exists) {
        return 0;
    }
    sm->transitions[sm->transition_count].from_state = from;
    sm->transitions[sm->transition_count].to_state = to;
    sm->transitions[sm->transition_count].input_char = input;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine* sm, const char* input) {
    if (sm == NULL || input == NULL) {
        return 0;
    }
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        int transition_found = 0;
        for (int j = 0; j < sm->transition_count; j++) {
            if (sm->transitions[j].from_state == sm->current_state && 
                sm->transitions[j].input_char == input[i]) {
                sm->current_state = sm->transitions[j].to_state;
                transition_found = 1;
                break;
            }
        }
        if (!transition_found) {
            return 0;
        }
    }
    return 1;
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

    State states_stack[4];
    states_stack[0].id = 0;
    strcpy(states_stack[0].name, "START");
    states_stack[1].id = 1;
    strcpy(states_stack[1].name, "PROCESSING");
    states_stack[2].id = 2;
    strcpy(states_stack[2].name, "ACCEPT");
    states_stack[3].id = 3;
    strcpy(states_stack[3].name, "REJECT");

    for (int i = 0; i < 4; i++) {
        if (!add_state(sm, states_stack[i].id, states_stack[i].name)) {
            printf("Failed to add state %d\n", states_stack[i].id);
            destroy_state_machine(sm);
            return 1;
        }
    }

    Transition transitions_stack[6];
    transitions_stack[0] = (Transition){0, 1, 'a'};
    transitions_stack[1] = (Transition){0, 3, 'b'};
    transitions_stack[2] = (Transition){1, 2, 'a'};
    transitions_stack[3] = (Transition){1, 3, 'b'};
    transitions_stack[4] = (Transition){2, 2, 'a'};
    transitions_stack[5] = (Transition){2,
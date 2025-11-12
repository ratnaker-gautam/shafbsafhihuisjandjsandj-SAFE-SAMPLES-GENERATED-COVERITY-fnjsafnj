//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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

int add_transition(StateMachine* sm, int from, int to, char input) {
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
    sm->transitions[sm->transition_count].input_char = input;
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine* sm, const char* input) {
    if (sm == NULL || input == NULL || sm->current_state == -1) {
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

    if (!add_state(sm, 0, "START") || !add_state(sm, 1, "READING") || 
        !add_state(sm, 2, "PROCESSING") || !add_state(sm, 3, "DONE")) {
        printf("Failed to add states\n");
        cleanup_state_machine(sm);
        return 1;
    }

    if (!add_transition(sm, 0, 1, 'B') || !add_transition(sm, 1, 1, 'A') || 
        !add_transition(sm, 1, 2, 'C') || !add_transition(sm, 2, 3, 'D') || 
        !add_transition(sm, 2, 1, 'A')) {
        printf("Failed to add transitions\n");
        cleanup_state_machine(sm);
        return 1;
    }

    sm->current_state = 0;

    char input[MAX_INPUT_LEN];
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Failed to read input\n");
        cleanup_state_machine(s
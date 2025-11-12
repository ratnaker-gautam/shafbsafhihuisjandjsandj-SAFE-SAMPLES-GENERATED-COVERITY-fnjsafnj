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
    char input_char;
} Transition;

typedef struct {
    int id;
    int is_final;
} State;

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

int add_state(StateMachine* sm, int id, int is_final) {
    if (sm == NULL || sm->state_count >= MAX_STATES) {
        return 0;
    }
    if (id < 0 || id >= MAX_STATES) {
        return 0;
    }
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == id) {
            return 0;
        }
    }
    sm->states[sm->state_count].id = id;
    sm->states[sm->state_count].is_final = is_final;
    sm->state_count++;
    return 1;
}

int add_transition(StateMachine* sm, int from, int to, char input) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from < 0 || from >= MAX_STATES || to < 0 || to >= MAX_STATES) {
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
    sm->current_state = 0;
    for (size_t i = 0; i < len; i++) {
        char current_char = input[i];
        int transition_found = 0;
        for (int j = 0; j < sm->transition_count; j++) {
            if (sm->transitions[j].from_state == sm->current_state && 
                sm->transitions[j].input_char == current_char) {
                sm->current_state = sm->transitions[j].to_state;
                transition_found = 1;
                break;
            }
        }
        if (!transition_found) {
            return 0;
        }
    }
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->states[i].id == sm->current_state && sm->states[i].is_final) {
            return 1;
        }
    }
    return 0;
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
    if (!add_state(sm, 0, 0) || !add_state(sm, 1, 0) || !add_state(sm, 2, 1)) {
        printf("Failed to add states\n");
        cleanup_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, 0, 1, 'a') || !add_transition(sm, 1, 2, 'b') || 
        !add_transition(sm, 2, 2, 'c')) {
        printf("Failed to add transitions\n");
        cleanup_state_machine(sm);
        return 1;
    }
    char input[MAX_INPUT_LEN];
    printf("Enter input string (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Failed to read input\n");
        cleanup_state_machine(sm);
        return 1;
    }
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    int result = process_input(sm, input);
    if (result) {
        printf("Input accepted - final state reached\n");
    } else {
        printf("Input rejected\n");
    }
    cleanup_state_machine(sm);
    return 0;
}
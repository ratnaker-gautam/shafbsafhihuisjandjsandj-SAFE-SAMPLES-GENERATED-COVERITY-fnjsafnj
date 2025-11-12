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

    add_state(sm, 0, "START");
    add_state(sm, 1, "READING_A");
    add_state(sm, 2, "READING_B");
    add_state(sm, 3, "ACCEPT");

    add_transition(sm, 0, 1, 'a');
    add_transition(sm, 1, 1, 'a');
    add_transition(sm, 1, 2, 'b');
    add_transition(sm, 2, 2, 'b');
    add_transition(sm, 2, 3, 'c');
    add_transition(sm, 3, 3, 'c');

    char input_buffer[MAX_INPUT_LEN];
    printf("Enter input sequence (a,b,c only): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        destroy_state_machine(sm);
        return 1;
    }
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
    }

    if (process_input(sm, input_buffer)) {
        printf("Input accepted. Final state: %s\n", get_current_state_name(sm));
    } else {
        printf("Input rejected. Final state:
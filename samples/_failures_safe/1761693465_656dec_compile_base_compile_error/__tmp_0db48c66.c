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

int add_state(StateMachine* sm, int id, const char* name) {
    if (sm == NULL || sm->state_count >= MAX_STATES) {
        return 0;
    }
    if (strlen(name) >= 32) {
        return 0;
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
        int found = 0;
        for (int j = 0; j < sm->transition_count; j++) {
            if (sm->transitions[j].from_state == sm->current_state && 
                sm->transitions[j].input_char == input[i]) {
                sm->current_state = sm->transitions[j].to_state;
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
    }
    return 1;
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
    
    State stack_states[3];
    stack_states[0].id = 0;
    strcpy(stack_states[0].name, "START");
    stack_states[1].id = 1;
    strcpy(stack_states[1].name, "MIDDLE");
    stack_states[2].id = 2;
    strcpy(stack_states[2].name, "END");
    
    for (int i = 0; i < 3; i++) {
        if (!add_state(sm, stack_states[i].id, stack_states[i].name)) {
            fprintf(stderr, "Failed to add state\n");
            cleanup_state_machine(sm);
            return 1;
        }
    }
    
    Transition stack_transitions[4];
    stack_transitions[0].from_state = 0;
    stack_transitions[0].to_state = 1;
    stack_transitions[0].input_char = 'a';
    stack_transitions[1].from_state = 1;
    stack_transitions[1].to_state = 2;
    stack_transitions[1].input_char = 'b';
    stack_transitions[2].from_state = 1;
    stack_transitions[2].to_state = 1;
    stack_transitions[2].input_char = 'c';
    stack_transitions[3].from_state = 2;
    stack_transitions[3].to_state = 0;
    stack_transitions[3].input_char = 'd';
    
    for (int i = 0; i < 4; i++) {
        if (!add_transition(sm, stack_transitions[i].from_state, 
                           stack_transitions[i].to_state, 
                           stack_transitions[i].input_char)) {
            fprintf(stderr, "Failed to add transition\n");
            cleanup_state_machine(sm);
            return 1;
        }
    }
    
    sm->current_state = 0;
    
    char input[MAX_INPUT_LEN];
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        cleanup_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (process_input(s
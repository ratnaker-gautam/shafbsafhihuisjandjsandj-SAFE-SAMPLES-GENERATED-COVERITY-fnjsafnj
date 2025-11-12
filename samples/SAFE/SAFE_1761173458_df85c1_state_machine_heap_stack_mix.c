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

void destroy_state_machine(StateMachine* sm) {
    free(sm);
}

int main() {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        printf("Failed to create state machine\n");
        return 1;
    }

    int state_stack[5];
    int stack_top = -1;

    add_state(sm, 0, "START");
    add_state(sm, 1, "PROCESSING");
    add_state(sm, 2, "VALIDATING");
    add_state(sm, 3, "COMPLETE");
    add_state(sm, 4, "ERROR");

    add_transition(sm, 0, 1, 's');
    add_transition(sm, 1, 2, 'v');
    add_transition(sm, 2, 3, 'c');
    add_transition(sm, 2, 4, 'e');
    add_transition(sm, 4, 1, 'r');

    sm->current_state = 0;

    printf("State Machine Simulation\n");
    printf("Available inputs: s(start), v(validate), c(complete), e(error), r(retry), q(quit)\n");

    char input_buffer[MAX_INPUT_LEN];
    while (1) {
        print_current_state(sm);
        printf("Enter input: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }

        if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
            continue;
        }

        char input_char = input_buffer[0];
        
        if (input_char == 'q') {
            break;
        }

        if (stack_top < 4) {
            state_stack[++stack_top] = sm->current_state;
        }

        if (!process_input(sm, input_char)) {
            printf("Invalid transition from current state\n");
            if (stack_top >= 0) {
                sm->current_state = state_stack[stack_top--];
            }
        }
    }

    destroy_state_machine(sm);
    return 0;
}
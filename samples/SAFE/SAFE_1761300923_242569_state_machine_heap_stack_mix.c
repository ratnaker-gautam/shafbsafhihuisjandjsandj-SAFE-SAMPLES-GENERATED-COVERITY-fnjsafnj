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

    int state_ids[] = {0, 1, 2, 3};
    char* state_names[] = {"START", "READING", "PROCESSING", "END"};
    for (int i = 0; i < 4; i++) {
        if (!add_state(sm, state_ids[i], state_names[i])) {
            printf("Failed to add state\n");
            destroy_state_machine(sm);
            return 1;
        }
    }

    struct {
        int from, to;
        char trigger;
    } transitions[] = {
        {0, 1, 'b'}, {1, 2, 'p'}, {2, 3, 'e'}, {2, 1, 'r'},
        {1, 1, 'c'}, {3, 0, 'r'}
    };

    for (int i = 0; i < 6; i++) {
        if (!add_transition(sm, transitions[i].from, transitions[i].to, transitions[i].trigger)) {
            printf("Failed to add transition\n");
            destroy_state_machine(sm);
            return 1;
        }
    }

    sm->current_state = 0;

    printf("State Machine Ready. Commands: b=begin, p=process, e=end, r=reset, c=continue, q=quit\n");

    char input[MAX_INPUT_LEN];
    while (1) {
        print_current_state(sm);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }

        if (strlen(input) > 0 && input[0] != '\n') {
            char cmd = input[0];
            if (process_input(sm, cmd)) {
                printf("Transition successful\n");
            } else {
                printf("Invalid transition from current state\n");
            }
        }
    }

    destroy_state_machine(sm);
    return 0;
}
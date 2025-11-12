//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define BUFFER_SIZE 256

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    State next;
    char trigger;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    int transition_count;
    char *name;
} StateMachine;

StateMachine* create_state_machine(const char *name) {
    if (name == NULL || strlen(name) == 0) return NULL;
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->name = malloc(strlen(name) + 1);
    if (sm->name == NULL) {
        free(sm);
        return NULL;
    }
    strcpy(sm->name, name);
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (sm->transitions == NULL) {
        free(sm->name);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    return sm;
}

int add_transition(StateMachine *sm, State from, State to, char trigger) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

int process_trigger(StateMachine *sm, char trigger) {
    if (sm == NULL) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    free(sm->name);
    free(sm->transitions);
    free(sm);
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *machine = create_state_machine("DemoMachine");
    if (machine == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(machine, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(machine, STATE_ACTIVE, STATE_PAUSED, 'p');
    add_transition(machine, STATE_ACTIVE, STATE_DONE, 'f');
    add_transition(machine, STATE_PAUSED, STATE_ACTIVE, 'r');
    add_transition(machine, STATE_PAUSED, STATE_ERROR, 'e');
    add_transition(machine, STATE_ERROR, STATE_IDLE, 'r');
    
    printf("State Machine: %s\n", machine->name);
    printf("Available triggers: s(start), p(pause), r(reset), f(finish), e(error)\n");
    printf("Current state: %s\n", state_to_string(machine->current_state));
    
    char input[BUFFER_SIZE];
    while (machine->current_state != STATE_DONE) {
        printf("Enter trigger: ");
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (strlen(input) == 0 || input[0] == '\n') continue;
        
        char trigger = input[0];
        if (process_trigger(machine, trigger)) {
            printf("Transition successful. New state: %s\n", 
                   state_to_string(machine->current_state));
        } else {
            printf("Invalid transition from state %s with trigger '%c'\n",
                   state_to_string(machine->current_state), trigger);
        }
    }
    
    printf("Final state: %s\n", state_to_string(machine->current_state));
    destroy_state_machine(machine);
    return 0;
}
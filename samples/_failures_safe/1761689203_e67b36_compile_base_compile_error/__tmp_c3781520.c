//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    char condition;
} Transition;

typedef struct {
    State state;
    char *name;
    int data;
} StateInfo;

typedef struct {
    StateInfo *heap_states;
    Transition stack_transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
} StateMachine;

int validate_state(State s) {
    return s >= STATE_IDLE && s <= STATE_DONE;
}

int add_transition(StateMachine *sm, State from, State to, char cond) {
    if (sm == NULL || !validate_state(from) || !validate_state(to)) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->stack_transitions[sm->transition_count].current = from;
    sm->stack_transitions[sm->transition_count].next = to;
    sm->stack_transitions[sm->transition_count].condition = cond;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, State current, char input) {
    if (sm == NULL || !validate_state(current)) return STATE_ERROR;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->stack_transitions[i].current == current && 
            sm->stack_transitions[i].condition == input) {
            return sm->stack_transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void cleanup_machine(StateMachine *sm) {
    if (sm != NULL && sm->heap_states != NULL) {
        for (int i = 0; i < sm->state_count; i++) {
            if (sm->heap_states[i].name != NULL) {
                free(sm->heap_states[i].name);
            }
        }
        free(sm->heap_states);
        sm->heap_states = NULL;
    }
}

int initialize_machine(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    sm->heap_states = malloc(MAX_STATES * sizeof(StateInfo));
    if (sm->heap_states == NULL) return 0;
    
    sm->state_count = 5;
    sm->transition_count = 0;
    
    const char *names[] = {"IDLE", "ACTIVE", "PAUSED", "ERROR", "DONE"};
    for (int i = 0; i < sm->state_count; i++) {
        sm->heap_states[i].state = (State)i;
        sm->heap_states[i].data = i * 10;
        size_t len = strlen(names[i]);
        sm->heap_states[i].name = malloc(len + 1);
        if (sm->heap_states[i].name == NULL) {
            cleanup_machine(sm);
            return 0;
        }
        strncpy(sm->heap_states[i].name, names[i], len);
        sm->heap_states[i].name[len] = '\0';
    }
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, 's')) return 0;
    if (!add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 'p')) return 0;
    if (!add_transition(sm, STATE_ACTIVE, STATE_DONE, 'f')) return 0;
    if (!add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 'r')) return 0;
    if (!add_transition(sm, STATE_PAUSED, STATE_ERROR, 'e')) return 0;
    if (!add_transition(sm, STATE_ERROR, STATE_IDLE, 'r')) return 0;
    
    return 1;
}

const char *get_state_name(StateMachine *sm, State s) {
    if (sm == NULL || !validate_state(s)) return "UNKNOWN";
    for (int i = 0; i < sm->state_count; i++) {
        if (sm->heap_states[i].state == s) {
            return sm->heap_states[i].name;
        }
    }
    return "UNKNOWN";
}

int main() {
    StateMachine machine = {0};
    
    if (!initialize_machine(&machine)) {
        fprintf(stderr, "Failed to initialize state machine\n");
        return 1;
    }
    
    State current = STATE_IDLE;
    char input[BUFFER_SIZE];
    
    printf("State Machine Simulation\n");
    printf("Available commands: s(start), p(pause), r(resume), f(finish), e(error), q(quit)\n");
    
    while (1) {
        printf("Current state: %s\n", get_state_name(&machine, current));
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }
        
        if (input[0] != '\n') {
            State next = process_input(&machine, current, input[0]);
            if (next
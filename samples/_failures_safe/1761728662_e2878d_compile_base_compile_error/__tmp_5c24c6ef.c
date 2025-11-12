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
    char condition[32];
} Transition;

typedef struct {
    State state;
    char name[32];
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

int add_transition(StateMachine *sm, State from, State to, const char *cond) {
    if (sm == NULL || !validate_state(from) || !validate_state(to) || cond == NULL) {
        return 0;
    }
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (strlen(cond) >= 32) {
        return 0;
    }
    
    Transition *t = &sm->stack_transitions[sm->transition_count];
    t->current = from;
    t->next = to;
    strncpy(t->condition, cond, 31);
    t->condition[31] = '\0';
    sm->transition_count++;
    return 1;
}

State process_state(StateMachine *sm, State current, const char *input) {
    if (sm == NULL || input == NULL || !validate_state(current)) {
        return STATE_ERROR;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->stack_transitions[i];
        if (t->current == current && strcmp(t->condition, input) == 0) {
            return t->next;
        }
    }
    return STATE_ERROR;
}

void cleanup_machine(StateMachine *sm) {
    if (sm != NULL && sm->heap_states != NULL) {
        free(sm->heap_states);
        sm->heap_states = NULL;
    }
}

int initialize_machine(StateMachine *sm) {
    if (sm == NULL) {
        return 0;
    }
    
    sm->state_count = 5;
    sm->heap_states = malloc(sizeof(StateInfo) * sm->state_count);
    if (sm->heap_states == NULL) {
        return 0;
    }
    
    StateInfo states[] = {
        {STATE_IDLE, "IDLE", 0},
        {STATE_ACTIVE, "ACTIVE", 1},
        {STATE_PAUSED, "PAUSED", 2},
        {STATE_ERROR, "ERROR", -1},
        {STATE_DONE, "DONE", 100}
    };
    
    for (int i = 0; i < sm->state_count; i++) {
        sm->heap_states[i] = states[i];
    }
    
    sm->transition_count = 0;
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start")) return 0;
    if (!add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause")) return 0;
    if (!add_transition(sm, STATE_ACTIVE, STATE_DONE, "complete")) return 0;
    if (!add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume")) return 0;
    if (!add_transition(sm, STATE_PAUSED, STATE_IDLE, "reset")) return 0;
    if (!add_transition(sm, STATE_ERROR, STATE_IDLE, "reset")) return 0;
    
    return 1;
}

const char* state_to_string(State s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine machine;
    if (!initialize_machine(&machine)) {
        fprintf(stderr, "Failed to initialize state machine\n");
        return 1;
    }
    
    State current = STATE_IDLE;
    char input[BUFFER_SIZE];
    
    printf("State Machine Demo\n");
    printf("Available commands: start, pause, resume, complete, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(current));
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            break;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        State next = process_state(&machine, current, input);
        if (next == STATE_ERROR) {
            printf("Invalid transition from %s with command '%s'\n", 
                   state_to_string(current), input);
        } else {
            current = next;
            printf("Transition
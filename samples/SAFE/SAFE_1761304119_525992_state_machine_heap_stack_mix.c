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
    STATE_COMPLETED,
    STATE_ERROR
} State;

typedef struct {
    State current;
    State next;
    char trigger[32];
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    int transition_count;
    char *buffer;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (!sm->transitions) {
        free(sm);
        return NULL;
    }
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    memset(sm->buffer, 0, BUFFER_SIZE);
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int add_transition(StateMachine *sm, State from, State to, const char *trigger) {
    if (!sm || !trigger) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->next = to;
    strncpy(t->trigger, trigger, 31);
    t->trigger[31] = '\0';
    
    sm->transition_count++;
    return 1;
}

int process_input(StateMachine *sm, const char *input) {
    if (!sm || !input) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && 
            strcmp(t->trigger, input) == 0) {
            sm->current_state = t->next;
            return 1;
        }
    }
    return 0;
}

const char* state_to_string(State s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_COMPLETED: return "COMPLETED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start");
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause");
    add_transition(sm, STATE_ACTIVE, STATE_COMPLETED, "finish");
    add_transition(sm, STATE_ACTIVE, STATE_ERROR, "error");
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume");
    add_transition(sm, STATE_PAUSED, STATE_IDLE, "reset");
    add_transition(sm, STATE_ERROR, STATE_IDLE, "reset");
    add_transition(sm, STATE_COMPLETED, STATE_IDLE, "reset");
    
    char input[64];
    
    printf("State Machine Demo\n");
    printf("Available commands: start, pause, resume, finish, error, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(sm->current_state));
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) break;
        
        if (strlen(input) == 0) continue;
        
        if (process_input(sm, input)) {
            printf("Transition successful\n");
        } else {
            printf("Invalid transition from current state\n");
        }
        printf("\n");
    }
    
    destroy_state_machine(sm);
    return 0;
}
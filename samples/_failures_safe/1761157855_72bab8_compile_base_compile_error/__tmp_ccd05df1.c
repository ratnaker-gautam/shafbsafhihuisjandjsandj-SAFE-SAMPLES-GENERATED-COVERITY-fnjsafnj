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
    State current_state;
    State next_state;
    char trigger[32];
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    char* stack_buffer;
    char* heap_buffer;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->state_count = 5;
    sm->states[0] = STATE_IDLE;
    sm->states[1] = STATE_ACTIVE;
    sm->states[2] = STATE_PAUSED;
    sm->states[3] = STATE_COMPLETED;
    sm->states[4] = STATE_ERROR;
    
    sm->transition_count = 6;
    
    sm->transitions[0].current_state = STATE_IDLE;
    sm->transitions[0].next_state = STATE_ACTIVE;
    strncpy(sm->transitions[0].trigger, "start", 31);
    sm->transitions[0].trigger[31] = '\0';
    
    sm->transitions[1].current_state = STATE_ACTIVE;
    sm->transitions[1].next_state = STATE_PAUSED;
    strncpy(sm->transitions[1].trigger, "pause", 31);
    sm->transitions[1].trigger[31] = '\0';
    
    sm->transitions[2].current_state = STATE_ACTIVE;
    sm->transitions[2].next_state = STATE_COMPLETED;
    strncpy(sm->transitions[2].trigger, "complete", 31);
    sm->transitions[2].trigger[31] = '\0';
    
    sm->transitions[3].current_state = STATE_PAUSED;
    sm->transitions[3].next_state = STATE_ACTIVE;
    strncpy(sm->transitions[3].trigger, "resume", 31);
    sm->transitions[3].trigger[31] = '\0';
    
    sm->transitions[4].current_state = STATE_ACTIVE;
    sm->transitions[4].next_state = STATE_ERROR;
    strncpy(sm->transitions[4].trigger, "error", 31);
    sm->transitions[4].trigger[31] = '\0';
    
    sm->transitions[5].current_state = STATE_ERROR;
    sm->transitions[5].next_state = STATE_IDLE;
    strncpy(sm->transitions[5].trigger, "reset", 31);
    sm->transitions[5].trigger[31] = '\0';
    
    sm->stack_buffer = NULL;
    sm->heap_buffer = malloc(BUFFER_SIZE);
    if (!sm->heap_buffer) {
        free(sm);
        return NULL;
    }
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm) {
        free(sm->heap_buffer);
        free(sm);
    }
}

State process_transition(StateMachine* sm, State current_state, const char* trigger) {
    if (!sm || !trigger) return current_state;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == current_state &&
            strcmp(sm->transitions[i].trigger, trigger) == 0) {
            return sm->transitions[i].next_state;
        }
    }
    return current_state;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_COMPLETED: return "COMPLETED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char stack_input[BUFFER_SIZE];
    State current_state = STATE_IDLE;
    
    printf("State Machine Simulation\n");
    printf("Available triggers: start, pause, resume, complete, error, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(current_state));
        printf("Enter trigger: ");
        
        if (!fgets(stack_input, sizeof(stack_input), stdin)) {
            fprintf(stderr, "Input error\n");
            break;
        }
        
        stack_input[strcspn(stack_input, "\n")] = '\0';
        
        if (strcmp(stack_input, "quit") == 0) {
            break;
        }
        
        if (strlen(stack_input) == 0) {
            continue;
        }
        
        strncpy(sm->heap_buffer, stack_input, BUFFER_SIZE - 1);
        sm->heap_buffer[BUFFER_SIZE - 1] = '\0';
        
        State new
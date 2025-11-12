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
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

StateMachine* create_state_machine() {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->state_count = 5;
    sm->states[0] = STATE_IDLE;
    sm->states[1] = STATE_ACTIVE;
    sm->states[2] = STATE_PAUSED;
    sm->states[3] = STATE_COMPLETED;
    sm->states[4] = STATE_ERROR;
    
    sm->transition_count = 6;
    
    strncpy(sm->transitions[0].trigger, "start", 31);
    sm->transitions[0].current = STATE_IDLE;
    sm->transitions[0].next = STATE_ACTIVE;
    
    strncpy(sm->transitions[1].trigger, "pause", 31);
    sm->transitions[1].current = STATE_ACTIVE;
    sm->transitions[1].next = STATE_PAUSED;
    
    strncpy(sm->transitions[2].trigger, "resume", 31);
    sm->transitions[2].current = STATE_PAUSED;
    sm->transitions[2].next = STATE_ACTIVE;
    
    strncpy(sm->transitions[3].trigger, "complete", 31);
    sm->transitions[3].current = STATE_ACTIVE;
    sm->transitions[3].next = STATE_COMPLETED;
    
    strncpy(sm->transitions[4].trigger, "error", 31);
    sm->transitions[4].current = STATE_ACTIVE;
    sm->transitions[4].next = STATE_ERROR;
    
    strncpy(sm->transitions[5].trigger, "reset", 31);
    sm->transitions[5].current = STATE_ERROR;
    sm->transitions[5].next = STATE_IDLE;
    
    sm->current_state = STATE_IDLE;
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm != NULL) {
        free(sm);
    }
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

int process_transition(StateMachine* sm, const char* trigger) {
    if (sm == NULL || trigger == NULL) {
        return 0;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (strcmp(sm->transitions[i].trigger, trigger) == 0 && 
            sm->transitions[i].current == sm->current_state) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    
    return 0;
}

void print_current_state(const StateMachine* sm) {
    if (sm != NULL) {
        printf("Current state: %s\n", state_to_string(sm->current_state));
    }
}

int main() {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    int running = 1;
    
    printf("State Machine Controller\n");
    printf("Available triggers: start, pause, resume, complete, error, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (running) {
        print_current_state(sm);
        printf("Enter trigger: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (process_transition(sm, input)) {
            printf("Transition successful: %s\n", input);
        } else {
            printf("Invalid transition from current state: %s\n", input);
        }
        
        printf("\n");
    }
    
    destroy_state_machine(sm);
    return 0;
}
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
    char trigger;
} Transition;

typedef struct {
    State current_state;
    Transition *transitions;
    size_t transition_count;
    char *buffer;
    size_t buffer_size;
} StateMachine;

int validate_transition(const StateMachine *sm, char trigger) {
    if (sm == NULL) return 0;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state &&
            sm->transitions[i].trigger == trigger) {
            return 1;
        }
    }
    return 0;
}

StateMachine* create_state_machine() {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (sm->transitions == NULL) {
        free(sm);
        return NULL;
    }
    
    sm->buffer = malloc(BUFFER_SIZE);
    if (sm->buffer == NULL) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->buffer_size = BUFFER_SIZE;
    
    Transition initial_transitions[] = {
        {STATE_IDLE, STATE_ACTIVE, 's'},
        {STATE_ACTIVE, STATE_PAUSED, 'p'},
        {STATE_ACTIVE, STATE_ERROR, 'e'},
        {STATE_ACTIVE, STATE_DONE, 'f'},
        {STATE_PAUSED, STATE_ACTIVE, 'r'},
        {STATE_PAUSED, STATE_ERROR, 'e'},
        {STATE_ERROR, STATE_IDLE, 'r'},
        {STATE_DONE, STATE_IDLE, 'r'}
    };
    
    size_t count = sizeof(initial_transitions) / sizeof(Transition);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (size_t i = 0; i < count; i++) {
        if (sm->transition_count < MAX_TRANSITIONS) {
            sm->transitions[sm->transition_count] = initial_transitions[i];
            sm->transition_count++;
        }
    }
    
    memset(sm->buffer, 0, BUFFER_SIZE);
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm != NULL) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

int process_trigger(StateMachine *sm, char trigger) {
    if (sm == NULL) return 0;
    
    if (!validate_transition(sm, trigger)) {
        return 0;
    }
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state &&
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            
            char state_str[32];
            switch (sm->current_state) {
                case STATE_IDLE: strcpy(state_str, "IDLE"); break;
                case STATE_ACTIVE: strcpy(state_str, "ACTIVE"); break;
                case STATE_PAUSED: strcpy(state_str, "PAUSED"); break;
                case STATE_ERROR: strcpy(state_str, "ERROR"); break;
                case STATE_DONE: strcpy(state_str, "DONE"); break;
                default: strcpy(state_str, "UNKNOWN"); break;
            }
            
            snprintf(sm->buffer, sm->buffer_size, 
                    "Transitioned to state: %s", state_str);
            return 1;
        }
    }
    return 0;
}

int main() {
    StateMachine *sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine Controller\n");
    printf("Available triggers: s(start), p(pause), r(reset), e(error), f(finish)\n");
    printf("Enter 'q' to quit\n\n");
    
    char input[16];
    while (1) {
        printf("Current state: ");
        switch (sm->current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_ACTIVE: printf("ACTIVE"); break;
            case STATE_PAUSED: printf("PAUSED"); break;
            case STATE_ERROR: printf("ERROR"); break;
            case STATE_DONE: printf("DONE"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\nEnter trigger: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }
        
        if (strlen(input) < 1 || input[0] == '\n') {
            continue;
        }
        
        char trigger = input[0
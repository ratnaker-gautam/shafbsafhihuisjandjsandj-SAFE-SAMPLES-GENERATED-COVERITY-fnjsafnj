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
        {STATE_ACTIVE, STATE_DONE, 'd'},
        {STATE_PAUSED, STATE_ACTIVE, 'r'},
        {STATE_PAUSED, STATE_ERROR, 'e'},
        {STATE_ERROR, STATE_IDLE, 'r'},
        {STATE_DONE, STATE_IDLE, 'r'}
    };
    
    size_t count = sizeof(initial_transitions) / sizeof(initial_transitions[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    memcpy(sm->transitions, initial_transitions, count * sizeof(Transition));
    sm->transition_count = count;
    
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
            
            size_t len = strlen(sm->buffer);
            if (len < sm->buffer_size - 2) {
                sm->buffer[len] = trigger;
                sm->buffer[len + 1] = '\0';
            }
            return 1;
        }
    }
    return 0;
}

void print_state_info(const StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "ACTIVE", "PAUSED", "ERROR", "DONE"
    };
    
    printf("Current state: %s\n", state_names[sm->current_state]);
    printf("Trigger history: %s\n", sm->buffer);
}

int main() {
    StateMachine *sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine Demo\n");
    printf("Available triggers: s(start), p(pause), r(reset), e(error), d(done)\n");
    printf("Enter 'q' to quit\n\n");
    
    char input[16];
    int running = 1;
    
    while (running) {
        print_state_info(sm);
        printf("Enter trigger: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            running = 0;
            continue;
        }
        
        if (input[0] != '\n') {
            if (process_trigger(sm, input[0])) {
                printf("Transition successful\n");
            } else {
                printf("Invalid transition for current state\n");
            }
        }
        
        printf("\n");
    }
    
    destroy_state_machine(sm);
    return 0;
}
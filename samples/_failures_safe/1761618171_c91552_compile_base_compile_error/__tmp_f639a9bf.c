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
    
    size_t count = sizeof(initial_transitions) / sizeof(initial_transitions[0]);
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
            
            if (sm->current_state == STATE_ACTIVE) {
                if (snprintf(sm->buffer, BUFFER_SIZE, "Processing data in state: ACTIVE") >= BUFFER_SIZE) {
                    sm->buffer[BUFFER_SIZE - 1] = '\0';
                }
            } else if (sm->current_state == STATE_PAUSED) {
                if (snprintf(sm->buffer, BUFFER_SIZE, "System paused") >= BUFFER_SIZE) {
                    sm->buffer[BUFFER_SIZE - 1] = '\0';
                }
            } else if (sm->current_state == STATE_ERROR) {
                if (snprintf(sm->buffer, BUFFER_SIZE, "Error occurred") >= BUFFER_SIZE) {
                    sm->buffer[BUFFER_SIZE - 1] = '\0';
                }
            } else if (sm->current_state == STATE_DONE) {
                if (snprintf(sm->buffer, BUFFER_SIZE, "Processing complete") >= BUFFER_SIZE) {
                    sm->buffer[BUFFER_SIZE - 1] = '\0';
                }
            } else {
                if (snprintf(sm->buffer, BUFFER_SIZE, "System idle") >= BUFFER_SIZE) {
                    sm->buffer[BUFFER_SIZE - 1] = '\0';
                }
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
    
    size_t state_index = sm->current_state;
    if (state_index >= sizeof(state_names) / sizeof(state_names[0])) {
        state_index = 0;
    }
    
    printf("Current state: %s\n", state_names[state_index]);
    printf("Status: %s\n", sm->buffer);
}

int main() {
    StateMachine *sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr
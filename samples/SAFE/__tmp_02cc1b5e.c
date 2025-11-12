//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
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

State get_next_state(const StateMachine *sm, char trigger) {
    if (sm == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state &&
            sm->transitions[i].trigger == trigger) {
            return sm->transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_buffer(StateMachine *sm) {
    if (sm == NULL || sm->buffer == NULL) return 0;
    
    size_t processed = 0;
    for (size_t i = 0; i < sm->buffer_size && sm->buffer[i] != '\0'; i++) {
        char trigger = sm->buffer[i];
        
        if (!validate_transition(sm, trigger)) {
            sm->current_state = STATE_ERROR;
            return 0;
        }
        
        State next_state = get_next_state(sm, trigger);
        if (next_state == STATE_ERROR) {
            sm->current_state = STATE_ERROR;
            return 0;
        }
        
        sm->current_state = next_state;
        processed++;
        
        if (sm->current_state == STATE_DONE) {
            break;
        }
    }
    
    return processed > 0;
}

StateMachine* create_state_machine(void) {
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
    
    Transition default_transitions[] = {
        {STATE_IDLE, STATE_ACTIVE, 's'},
        {STATE_ACTIVE, STATE_PAUSED, 'p'},
        {STATE_ACTIVE, STATE_DONE, 'e'},
        {STATE_PAUSED, STATE_ACTIVE, 'r'},
        {STATE_PAUSED, STATE_ERROR, 'x'},
        {STATE_ERROR, STATE_IDLE, 'r'}
    };
    
    size_t count = sizeof(default_transitions) / sizeof(default_transitions[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    memcpy(sm->transitions, default_transitions, count * sizeof(Transition));
    sm->transition_count = count;
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm != NULL) {
        free(sm->transitions);
        free(sm->buffer);
        free(sm);
    }
}

const char* state_to_string(State state) {
    switch(state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    printf("State Machine Simulator\n");
    printf("Available triggers: s(start), p(pause), r(reset), e(end), x(error)\n");
    printf("Current state: %s\n", state_to_string(sm->current_state));
    
    char input[BUFFER_SIZE];
    printf("Enter sequence of triggers: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= BUFFER_SIZE) {
        fprintf(stderr, "Input too long\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    strncpy(sm->buffer, input, BUFFER_SIZE -
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
        
        if (sm->current_state == STATE_DONE) {
            return 1;
        }
    }
    
    return sm->current_state == STATE_DONE;
}

int initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return 0;
    
    sm->transition_count = 5;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (sm->transitions == NULL) return 0;
    
    sm->transitions[0] = (Transition){STATE_IDLE, STATE_ACTIVE, 's'};
    sm->transitions[1] = (Transition){STATE_ACTIVE, STATE_PAUSED, 'p'};
    sm->transitions[2] = (Transition){STATE_ACTIVE, STATE_DONE, 'e'};
    sm->transitions[3] = (Transition){STATE_PAUSED, STATE_ACTIVE, 'r'};
    sm->transitions[4] = (Transition){STATE_PAUSED, STATE_ERROR, 'x'};
    
    sm->current_state = STATE_IDLE;
    
    sm->buffer_size = BUFFER_SIZE;
    sm->buffer = malloc(sm->buffer_size);
    if (sm->buffer == NULL) {
        free(sm->transitions);
        return 0;
    }
    
    return 1;
}

void cleanup_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    if (sm->transitions != NULL) {
        free(sm->transitions);
    }
    
    if (sm->buffer != NULL) {
        free(sm->buffer);
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
    StateMachine sm_stack;
    StateMachine *sm_heap = malloc(sizeof(StateMachine));
    
    if (sm_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (!initialize_state_machine(&sm_stack)) {
        fprintf(stderr, "Failed to initialize stack state machine\n");
        free(sm_heap);
        return 1;
    }
    
    if (!initialize_state_machine(sm_heap)) {
        fprintf(stderr, "Failed to initialize heap state machine\n");
        cleanup_state_machine(&sm_stack);
        free(sm_heap);
        return 1;
    }
    
    printf("Enter command sequence (s=start, p=pause, r=resume, e=end, x=error): ");
    
    char input[BUFFER_SIZE];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        cleanup_state_machine(&sm_stack);
        cleanup_state_machine(sm_heap);
        free(sm_heap);
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= BUFFER
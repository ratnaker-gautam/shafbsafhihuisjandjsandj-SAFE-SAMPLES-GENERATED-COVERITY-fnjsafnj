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
    STATE_PROCESSING,
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

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
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
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    sm->buffer_size = BUFFER_SIZE;
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

int add_transition(StateMachine *sm, State from, State to, char trigger) {
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].next = to;
    sm->transitions[sm->transition_count].trigger = trigger;
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine *sm, char trigger) {
    if (!sm) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].trigger == trigger) {
            sm->current_state = sm->transitions[i].next;
            return sm->current_state;
        }
    }
    return STATE_ERROR;
}

void process_buffer(StateMachine *sm) {
    if (!sm || !sm->buffer) return;
    
    for (size_t i = 0; i < sm->buffer_size && sm->buffer[i] != '\0'; i++) {
        State new_state = process_trigger(sm, sm->buffer[i]);
        if (new_state == STATE_ERROR) {
            printf("Invalid transition from state %d with trigger '%c'\n", 
                   sm->current_state, sm->buffer[i]);
            return;
        }
        
        printf("Processed '%c', moved to state %d\n", sm->buffer[i], new_state);
        
        if (new_state == STATE_DONE) {
            printf("Reached final state\n");
            return;
        }
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, 's');
    add_transition(sm, STATE_ACTIVE, STATE_PROCESSING, 'p');
    add_transition(sm, STATE_PROCESSING, STATE_DONE, 'd');
    add_transition(sm, STATE_PROCESSING, STATE_ERROR, 'e');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'r');
    
    printf("Enter sequence of characters (max %d): ", BUFFER_SIZE - 1);
    
    if (fgets(sm->buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
    }
    
    printf("Initial state: %d\n", sm->current_state);
    printf("Processing buffer: %s\n", sm->buffer);
    
    process_buffer(sm);
    
    printf("Final state: %d\n", sm->current_state);
    
    destroy_state_machine(sm);
    return 0;
}
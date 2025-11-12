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
    
    sm->buffer_size = BUFFER_SIZE;
    sm->buffer = malloc(sm->buffer_size);
    if (!sm->buffer) {
        free(sm->transitions);
        free(sm);
        return NULL;
    }
    memset(sm->buffer, 0, sm->buffer_size);
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->buffer);
        free(sm->transitions);
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
            break;
        }
    }
    
    return sm->current_state;
}

void process_buffer(StateMachine *sm) {
    if (!sm || !sm->buffer) return;
    
    for (size_t i = 0; i < sm->buffer_size && sm->buffer[i] != '\0'; i++) {
        State new_state = process_trigger(sm, sm->buffer[i]);
        
        switch (new_state) {
            case STATE_IDLE:
                printf("State: IDLE\n");
                break;
            case STATE_ACTIVE:
                printf("State: ACTIVE\n");
                break;
            case STATE_PAUSED:
                printf("State: PAUSED\n");
                break;
            case STATE_ERROR:
                printf("State: ERROR\n");
                return;
            case STATE_DONE:
                printf("State: DONE\n");
                return;
            default:
                break;
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
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 'p');
    add_transition(sm, STATE_ACTIVE, STATE_DONE, 'e');
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 'r');
    add_transition(sm, STATE_PAUSED, STATE_ERROR, 'x');
    add_transition(sm, STATE_ERROR, STATE_IDLE, 'r');
    
    printf("Enter command sequence (max %d chars): ", BUFFER_SIZE - 1);
    
    if (fgets(sm->buffer, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Failed to read input\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    size_t len = strlen(sm->buffer);
    if (len > 0 && sm->buffer[len - 1] == '\n') {
        sm->buffer[len - 1] = '\0';
    }
    
    printf("Processing commands:\n");
    process_buffer(sm);
    
    destroy_state_machine(sm);
    return 0;
}
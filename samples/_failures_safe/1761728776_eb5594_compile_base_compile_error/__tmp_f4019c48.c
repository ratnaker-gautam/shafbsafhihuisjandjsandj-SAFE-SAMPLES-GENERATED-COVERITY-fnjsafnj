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
    State from;
    State to;
    char trigger[32];
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
    sm->buffer[0] = '\0';
    
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
    if (!sm || sm->transition_count >= MAX_TRANSITIONS) return 0;
    if (!trigger || strlen(trigger) >= 32) return 0;
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->from = from;
    t->to = to;
    strncpy(t->trigger, trigger, 31);
    t->trigger[31] = '\0';
    
    sm->transition_count++;
    return 1;
}

int process_trigger(StateMachine *sm, const char *trigger) {
    if (!sm || !trigger) return 0;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->from == sm->current_state && strcmp(t->trigger, trigger) == 0) {
            sm->current_state = t->to;
            return 1;
        }
    }
    return 0;
}

void process_buffer(StateMachine *sm) {
    if (!sm || !sm->buffer) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (sm->buffer[0] != '\0') {
                sm->current_state = STATE_ACTIVE;
            }
            break;
        case STATE_ACTIVE:
            if (strlen(sm->buffer) > 100) {
                sm->current_state = STATE_ERROR;
            } else if (sm->buffer[0] == 'p') {
                sm->current_state = STATE_PAUSED;
            } else if (sm->buffer[0] == 'd') {
                sm->current_state = STATE_DONE;
            }
            break;
        case STATE_PAUSED:
            if (sm->buffer[0] == 'r') {
                sm->current_state = STATE_ACTIVE;
            } else if (sm->buffer[0] == 'd') {
                sm->current_state = STATE_DONE;
            }
            break;
        case STATE_ERROR:
            if (sm->buffer[0] == 'r') {
                sm->current_state = STATE_IDLE;
            }
            break;
        case STATE_DONE:
            break;
    }
}

const char* state_to_string(State state) {
    switch (state) {
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
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start");
    add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause");
    add_transition(sm, STATE_ACTIVE, STATE_DONE, "complete");
    add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume");
    add_transition(sm, STATE_PAUSED, STATE_DONE, "complete");
    add_transition(sm, STATE_ERROR, STATE_IDLE, "reset");
    
    char input[64];
    
    printf("State Machine Demo\n");
    printf("Available triggers: start, pause, resume, complete, reset\n");
    printf("Current state: %s\n", state_to_string(sm->current_state));
    
    while (sm->current_state != STATE_DONE) {
        printf("\nEnter trigger: ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) >= sizeof(input) -
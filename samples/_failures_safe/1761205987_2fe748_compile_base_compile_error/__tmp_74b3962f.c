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

int validate_transition(const Transition *t) {
    if (t == NULL) return 0;
    if (t->current >= STATE_DONE + 1) return 0;
    if (t->next >= STATE_DONE + 1) return 0;
    return 1;
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
    memset(sm->buffer, 0, BUFFER_SIZE);
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    free(sm->transitions);
    free(sm->buffer);
    free(sm);
}

int add_transition(StateMachine *sm, State from, State to, char trigger) {
    if (sm == NULL) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    if (from >= STATE_DONE + 1) return 0;
    if (to >= STATE_DONE + 1) return 0;
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->next = to;
    t->trigger = trigger;
    
    if (!validate_transition(t)) return 0;
    
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine *sm, char trigger) {
    if (sm == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && t->trigger == trigger) {
            sm->current_state = t->next;
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

int safe_strcpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) return 0;
    
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

void process_buffer(StateMachine *sm) {
    if (sm == NULL || sm->buffer == NULL) return;
    
    for (size_t i = 0; i < sm->buffer_size && sm->buffer[i] != '\0'; i++) {
        State new_state = process_trigger(sm, sm->buffer[i]);
        if (new_state == STATE_ERROR) {
            break;
        }
    }
}

const char* state_to_string(State s) {
    switch(s) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine *sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, 's')) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_ACTIVE, STATE_PAUSED, 'p')) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_PAUSED, STATE_ACTIVE, 'r')) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_ACTIVE, STATE_DONE, 'e')) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine(sm);
        return 1;
    }
    if (!add_transition(sm, STATE_PAUSED, STATE_DONE, 'e')) {
        fprintf(stderr, "Failed to add transition\n");
        destroy_state_machine
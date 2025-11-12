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
    if (!sm || !trigger) return 0;
    if (sm->transition_count >= MAX_TRANSITIONS) return 0;
    if (strlen(trigger) >= 32) return 0;
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current = from;
    t->next = to;
    strncpy(t->trigger, trigger, 31);
    t->trigger[31] = '\0';
    sm->transition_count++;
    return 1;
}

State process_trigger(StateMachine *sm, const char *trigger) {
    if (!sm || !trigger) return sm ? sm->current_state : STATE_ERROR;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current == sm->current_state && strcmp(t->trigger, trigger) == 0) {
            sm->current_state = t->next;
            break;
        }
    }
    
    return sm->current_state;
}

int append_to_buffer(StateMachine *sm, const char *data) {
    if (!sm || !data) return 0;
    
    size_t data_len = strlen(data);
    size_t current_len = strlen(sm->buffer);
    
    if (current_len + data_len + 1 > sm->buffer_size) {
        size_t new_size = sm->buffer_size * 2;
        if (new_size < sm->buffer_size) return 0;
        
        char *new_buffer = realloc(sm->buffer, new_size);
        if (!new_buffer) return 0;
        
        sm->buffer = new_buffer;
        sm->buffer_size = new_size;
    }
    
    strncat(sm->buffer, data, sm->buffer_size - current_len - 1);
    return 1;
}

void process_state_action(StateMachine *sm) {
    if (!sm) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            append_to_buffer(sm, "[IDLE]");
            break;
        case STATE_ACTIVE:
            append_to_buffer(sm, "[ACTIVE]");
            break;
        case STATE_PAUSED:
            append_to_buffer(sm, "[PAUSED]");
            break;
        case STATE_ERROR:
            append_to_buffer(sm, "[ERROR]");
            break;
        case STATE_DONE:
            append_to_buffer(sm, "[DONE]");
            break;
    }
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    if (!add_transition(sm, STATE_IDLE, STATE_ACTIVE, "start") ||
        !add_transition(sm, STATE_ACTIVE, STATE_PAUSED, "pause") ||
        !add_transition(sm, STATE_PAUSED, STATE_ACTIVE, "resume") ||
        !add_transition(sm, STATE_ACTIVE, STATE_DONE, "complete") ||
        !add_transition(sm, STATE_ACTIVE, STATE_ERROR, "error") ||
        !add_transition(sm, STATE_ERROR, STATE_IDLE, "reset") ||
        !add_transition(sm, STATE_DONE, STATE_IDLE, "reset")) {
        fprintf(stderr, "Failed to setup transitions\n");
        destroy_state_machine(sm);
        return 1;
    }
    
    const char *triggers[] = {"start", "pause", "resume", "complete", "error", "reset", NULL};
    
    for (int i = 0; triggers[i] != NULL; i++) {
        process_state_action(sm);
        printf("Current state: %d, Processing trigger: %s\n", sm->current_state, triggers[i]);
        process_trigger(sm, triggers[i
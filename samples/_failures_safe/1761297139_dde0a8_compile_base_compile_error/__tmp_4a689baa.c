//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct EventData {
    int value;
    char message[32];
};

struct StateMachine {
    enum State current_state;
    int counter;
    char buffer[64];
};

struct StateTransition {
    enum State from_state;
    int event_type;
    enum State to_state;
    int (*action)(struct StateMachine*, struct EventData*);
};

int action_idle_to_ready(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    if (ed->value < 0 || ed->value > 100) return 0;
    sm->counter = ed->value;
    snprintf(sm->buffer, sizeof(sm->buffer), "Ready: %d", ed->value);
    return 1;
}

int action_ready_to_processing(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    if (strlen(ed->message) >= sizeof(sm->buffer)) return 0;
    strncpy(sm->buffer, ed->message, sizeof(sm->buffer) - 1);
    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
    sm->counter *= 2;
    if (sm->counter < 0) return 0;
    return 1;
}

int action_processing_to_complete(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    if (sm->counter > 1000) return 0;
    sm->counter += 100;
    snprintf(sm->buffer, sizeof(sm->buffer), "Complete: %d", sm->counter);
    return 1;
}

int action_error(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL) return 0;
    sm->counter = -1;
    strncpy(sm->buffer, "Error state", sizeof(sm->buffer) - 1);
    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
    return 1;
}

struct StateTransition transitions[] = {
    {STATE_IDLE, 1, STATE_READY, action_idle_to_ready},
    {STATE_READY, 2, STATE_PROCESSING, action_ready_to_processing},
    {STATE_PROCESSING, 3, STATE_COMPLETE, action_processing_to_complete},
    {STATE_IDLE, 9, STATE_ERROR, action_error},
    {STATE_READY, 9, STATE_ERROR, action_error},
    {STATE_PROCESSING, 9, STATE_ERROR, action_error},
    {STATE_COMPLETE, 9, STATE_ERROR, action_error}
};

int process_event(struct StateMachine* sm, int event_type, struct EventData* ed) {
    if (sm == NULL) return 0;
    
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].from_state == sm->current_state && 
            transitions[i].event_type == event_type) {
            
            if (transitions[i].action(sm, ed)) {
                sm->current_state = transitions[i].to_state;
                return 1;
            }
            return 0;
        }
    }
    return 0;
}

void print_state(const struct StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("State: %s, Counter: %d, Buffer: %s\n", 
           state_names[sm->current_state], sm->counter, sm->buffer);
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.counter = 0;
    strncpy(sm.buffer, "Initial", sizeof(sm.buffer) - 1);
    sm.buffer[sizeof(sm.buffer) - 1] = '\0';
    
    printf("Initial state:\n");
    print_state(&sm);
    
    struct EventData ed1;
    ed1.value = 42;
    strncpy(ed1.message, "Start processing", sizeof(ed1.message) - 1);
    ed1.message[sizeof(ed1.message) - 1] = '\0';
    
    if (process_event(&sm, 1, &ed1)) {
        printf("Event 1 processed:\n");
        print_state(&sm);
    }
    
    struct EventData ed2;
    ed2.value = 0;
    strncpy(ed2.message, "Processing data", sizeof(ed2.message) - 1);
    ed2.message[sizeof(ed2.message) - 1] = '\0';
    
    if (process_event(&sm, 2, &ed2)) {
        printf("Event 2 processed:\n");
        print_state(&sm);
    }
    
    struct EventData ed3;
    ed3.value = 0;
    strncpy(ed3.message, "", sizeof(ed3.message) - 1);
    ed3.message[sizeof(ed
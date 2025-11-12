//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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

struct Event {
    int type;
    char data[32];
};

struct Context {
    enum State current_state;
    uint32_t counter;
    char buffer[64];
};

struct StateMachine {
    struct Context context;
    void (*transition)(struct StateMachine*, struct Event*);
};

void handle_idle(struct StateMachine* sm, struct Event* evt);
void handle_ready(struct StateMachine* sm, struct Event* evt);
void handle_processing(struct StateMachine* sm, struct Event* evt);
void handle_complete(struct StateMachine* sm, struct Event* evt);
void handle_error(struct StateMachine* sm, struct Event* evt);

void state_machine_init(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    sm->transition = handle_idle;
}

void handle_idle(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 1) {
        if (strlen(evt->data) < sizeof(sm->context.buffer)) {
            strncpy(sm->context.buffer, evt->data, sizeof(sm->context.buffer)-1);
            sm->context.buffer[sizeof(sm->context.buffer)-1] = '\0';
            sm->context.current_state = STATE_READY;
            sm->transition = handle_ready;
        } else {
            sm->context.current_state = STATE_ERROR;
            sm->transition = handle_error;
        }
    }
}

void handle_ready(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 2) {
        sm->context.counter = 0;
        sm->context.current_state = STATE_PROCESSING;
        sm->transition = handle_processing;
    } else if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        sm->transition = handle_idle;
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 3) {
        if (sm->context.counter < 1000) {
            sm->context.counter++;
        }
        if (sm->context.counter >= 5) {
            sm->context.current_state = STATE_COMPLETE;
            sm->transition = handle_complete;
        }
    } else if (evt->type == 4) {
        sm->context.current_state = STATE_ERROR;
        sm->transition = handle_error;
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        sm->transition = handle_idle;
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        sm->transition = handle_idle;
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct Event events[] = {
        {1, "test_data"},
        {2, ""},
        {3, ""},
        {3, ""},
        {3, ""},
        {3, ""},
        {3, ""},
        {0, ""}
    };
    
    int num_events = sizeof(events) / sizeof(events[0]);
    
    for (int i = 0; i < num_events; i++) {
        if (sm.transition != NULL) {
            sm.transition(&sm, &events[i]);
        }
        
        printf("State: %d, Counter: %u, Buffer: %s\n", 
               sm.context.current_state, 
               sm.context.counter, 
               sm.context.buffer);
    }
    
    return 0;
}
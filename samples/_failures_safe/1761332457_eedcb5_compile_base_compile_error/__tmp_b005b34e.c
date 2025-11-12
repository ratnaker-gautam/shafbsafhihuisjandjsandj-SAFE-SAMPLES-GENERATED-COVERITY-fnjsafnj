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
    void (*handler)(struct StateMachine*, struct Event*);
};

void handle_idle(struct StateMachine* sm, struct Event* evt);
void handle_ready(struct StateMachine* sm, struct Event* evt);
void handle_processing(struct StateMachine* sm, struct Event* evt);
void handle_complete(struct StateMachine* sm, struct Event* evt);
void handle_error(struct StateMachine* sm, struct Event* evt);

void transition_state(struct StateMachine* sm, enum State new_state) {
    if (sm == NULL) return;
    sm->context.current_state = new_state;
    
    switch (new_state) {
        case STATE_IDLE:
            sm->handler = handle_idle;
            break;
        case STATE_READY:
            sm->handler = handle_ready;
            break;
        case STATE_PROCESSING:
            sm->handler = handle_processing;
            break;
        case STATE_COMPLETE:
            sm->handler = handle_complete;
            break;
        case STATE_ERROR:
            sm->handler = handle_error;
            break;
        default:
            return;
    }
}

void handle_idle(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 1) {
        if (strlen(evt->data) < sizeof(sm->context.buffer)) {
            strncpy(sm->context.buffer, evt->data, sizeof(sm->context.buffer) - 1);
            sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
            transition_state(sm, STATE_READY);
        } else {
            transition_state(sm, STATE_ERROR);
        }
    }
}

void handle_ready(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 2) {
        sm->context.counter = 0;
        transition_state(sm, STATE_PROCESSING);
    } else if (evt->type == 0) {
        transition_state(sm, STATE_IDLE);
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 3) {
        if (sm->context.counter < 1000) {
            size_t len = strlen(sm->context.buffer);
            if (len < sizeof(sm->context.buffer) - 1) {
                sm->context.buffer[len] = 'X';
                sm->context.buffer[len + 1] = '\0';
            }
            sm->context.counter++;
        }
        
        if (sm->context.counter >= 5) {
            transition_state(sm, STATE_COMPLETE);
        }
    } else if (evt->type == 4) {
        transition_state(sm, STATE_ERROR);
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 0) {
        sm->context.buffer[0] = '\0';
        sm->context.counter = 0;
        transition_state(sm, STATE_IDLE);
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 0) {
        sm->context.buffer[0] = '\0';
        sm->context.counter = 0;
        transition_state(sm, STATE_IDLE);
    }
}

void init_state_machine(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    sm->context.buffer[0] = '\0';
    sm->handler = handle_idle;
}

void print_state_info(struct StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->context.current_state]);
    printf("Counter: %u\n", sm->context.counter);
    printf("Buffer: %s\n", sm->context.buffer);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    init_state_machine(&sm);
    
    struct Event events[] = {
        {1, "Hello"},
        {2, ""},
        {3, ""},
        {3, ""},
        {3, ""},
        {3, ""},
        {3, ""},
        {0, ""}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num
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

void handle_idle(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL) return;
    
    switch (evt->type) {
        case 1:
            if (strlen(evt->data) < sizeof(sm->context.buffer)) {
                strncpy(sm->context.buffer, evt->data, sizeof(sm->context.buffer) - 1);
                sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
                sm->context.current_state = STATE_READY;
                sm->handler = handle_ready;
            } else {
                sm->context.current_state = STATE_ERROR;
                sm->handler = handle_error;
            }
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
    }
}

void handle_ready(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL) return;
    
    switch (evt->type) {
        case 2:
            sm->context.counter = 0;
            sm->context.current_state = STATE_PROCESSING;
            sm->handler = handle_processing;
            break;
        case 3:
            sm->context.buffer[0] = '\0';
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL) return;
    
    switch (evt->type) {
        case 4:
            if (sm->context.counter < 100) {
                sm->context.counter++;
            }
            if (sm->context.counter >= 5) {
                sm->context.current_state = STATE_COMPLETE;
                sm->handler = handle_complete;
            }
            break;
        case 5:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
        default:
            break;
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL) return;
    
    switch (evt->type) {
        case 6:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            sm->context.buffer[0] = '\0';
            sm->context.counter = 0;
            break;
        default:
            break;
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL) return;
    
    switch (evt->type) {
        case 7:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            sm->context.buffer[0] = '\0';
            sm->context.counter = 0;
            break;
        default:
            break;
    }
}

void state_machine_init(struct StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    sm->context.buffer[0] = '\0';
    sm->handler = handle_idle;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct Event events[] = {
        {1, "test_data"},
        {2, ""},
        {4, ""},
        {4, ""},
        {4, ""},
        {4, ""},
        {4, ""},
        {6, ""}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events; i++) {
        printf("Processing event %zu, type %d\n", i + 1, events[i].type);
        printf("Current state: %d\n", sm.context.current_state);
        
        sm.handler(&sm, &events[i]);
        
        printf("New state: %d\n", sm.context.current_state);
        printf("Counter: %u\n", sm.context.counter);
        printf("Buffer: %s\n", sm.context.buffer);
        printf("---\n");
    }
    
    return 0;
}
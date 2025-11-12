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
    if (sm == NULL) return;
    
    switch (evt->type) {
        case 1:
            if (strlen(evt->data) < sizeof(sm->context.buffer)) {
                strncpy(sm->context.buffer, evt->data, sizeof(sm->context.buffer) - 1);
                sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
                sm->context.current_state = STATE_READY;
                sm->handler = handle_ready;
                sm->context.counter = 0;
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
    if (sm == NULL) return;
    
    switch (evt->type) {
        case 2:
            sm->context.current_state = STATE_PROCESSING;
            sm->handler = handle_processing;
            break;
        case 9:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL) return;
    if (sm == NULL) return;
    
    switch (evt->type) {
        case 3:
            if (sm->context.counter < 1000) {
                sm->context.counter++;
            }
            break;
        case 4:
            if (sm->context.counter >= 5) {
                sm->context.current_state = STATE_COMPLETE;
                sm->handler = handle_complete;
            } else {
                sm->context.current_state = STATE_ERROR;
                sm->handler = handle_error;
            }
            break;
        case 9:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
            sm->context.counter = 0;
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL) return;
    if (sm == NULL) return;
    
    switch (evt->type) {
        case 5:
            printf("Result: %s processed %u times\n", sm->context.buffer, sm->context.counter);
            break;
        case 9:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
            sm->context.counter = 0;
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL) return;
    if (sm == NULL) return;
    
    switch (evt->type) {
        case 9:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
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
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    sm->handler = handle_idle;
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
        {
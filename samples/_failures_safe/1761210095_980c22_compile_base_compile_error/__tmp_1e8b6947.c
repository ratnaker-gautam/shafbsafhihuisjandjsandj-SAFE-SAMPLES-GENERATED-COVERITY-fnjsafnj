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
    void (*transition)(struct StateMachine*, struct Event*);
};

void handle_idle(struct StateMachine *sm, struct Event *evt);
void handle_ready(struct StateMachine *sm, struct Event *evt);
void handle_processing(struct StateMachine *sm, struct Event *evt);
void handle_complete(struct StateMachine *sm, struct Event *evt);
void handle_error(struct StateMachine *sm, struct Event *evt);

void handle_idle(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 1:
            sm->context.current_state = STATE_READY;
            strncpy(sm->context.buffer, "Initialized", sizeof(sm->context.buffer) - 1);
            sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
            break;
        case 2:
            sm->context.current_state = STATE_ERROR;
            strncpy(sm->context.buffer, "Invalid start", sizeof(sm->context.buffer) - 1);
            sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
            break;
        default:
            break;
    }
}

void handle_ready(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 3:
            sm->context.current_state = STATE_PROCESSING;
            sm->context.counter = 0;
            strncpy(sm->context.buffer, evt->data, sizeof(sm->context.buffer) - 1);
            sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
            break;
        case 4:
            sm->context.current_state = STATE_IDLE;
            sm->context.buffer[0] = '\0';
            break;
        default:
            break;
    }
}

void handle_processing(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 5:
            if (sm->context.counter < 10) {
                sm->context.counter++;
            }
            if (sm->context.counter >= 5) {
                sm->context.current_state = STATE_COMPLETE;
                strncpy(sm->context.buffer, "Processing done", sizeof(sm->context.buffer) - 1);
                sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
            }
            break;
        case 6:
            sm->context.current_state = STATE_ERROR;
            strncpy(sm->context.buffer, "Processing failed", sizeof(sm->context.buffer) - 1);
            sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
            break;
        default:
            break;
    }
}

void handle_complete(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 7:
            sm->context.current_state = STATE_IDLE;
            sm->context.counter = 0;
            sm->context.buffer[0] = '\0';
            break;
        default:
            break;
    }
}

void handle_error(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 8:
            sm->context.current_state = STATE_IDLE;
            sm->context.counter = 0;
            sm->context.buffer[0] = '\0';
            break;
        default:
            break;
    }
}

void state_machine_transition(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (sm->context.current_state) {
        case STATE_IDLE:
            handle_idle(sm, evt);
            break;
        case STATE_READY:
            handle_ready(sm, evt);
            break;
        case STATE_PROCESSING:
            handle_processing(sm, evt);
            break;
        case STATE_COMPLETE:
            handle_complete(sm, evt);
            break;
        case STATE_ERROR:
            handle_error(sm, evt);
            break;
        default:
            break;
    }
}

void print_state_info(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    int state_index = sm->context.current_state;
    if (state_index >= 0 && state_index <= 4) {
        printf("State: %s, Counter: %u, Buffer: %
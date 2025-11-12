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
    if (sm == NULL || evt == NULL) return;
    if (evt->type == 1) {
        sm->context.current_state = STATE_READY;
        strncpy(sm->context.buffer, "Ready", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    } else {
        sm->context.current_state = STATE_ERROR;
        strncpy(sm->context.buffer, "Invalid event", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    }
}

void handle_ready(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    if (evt->type == 2) {
        sm->context.current_state = STATE_PROCESSING;
        strncpy(sm->context.buffer, evt->data, sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
        sm->context.counter = 0;
    } else if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        strncpy(sm->context.buffer, "Idle", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    } else {
        sm->context.current_state = STATE_ERROR;
        strncpy(sm->context.buffer, "Invalid event", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    if (evt->type == 3) {
        if (sm->context.counter < 10) {
            sm->context.counter++;
            if (sm->context.counter >= 10) {
                sm->context.current_state = STATE_COMPLETE;
                strncpy(sm->context.buffer, "Complete", sizeof(sm->context.buffer) - 1);
                sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
            }
        }
    } else if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        strncpy(sm->context.buffer, "Idle", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    } else {
        sm->context.current_state = STATE_ERROR;
        strncpy(sm->context.buffer, "Invalid event", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        strncpy(sm->context.buffer, "Idle", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    } else {
        sm->context.current_state = STATE_ERROR;
        strncpy(sm->context.buffer, "Invalid event", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        strncpy(sm->context.buffer, "Idle", sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    }
}

void state_machine_init(struct StateMachine* sm) {
    if (sm == NULL)
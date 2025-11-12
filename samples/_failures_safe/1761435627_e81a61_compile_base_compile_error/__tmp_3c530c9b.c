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
    int counter;
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
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 2:
            if (sm->context.counter < 10) {
                sm->context.current_state = STATE_PROCESSING;
                sm->handler = handle_processing;
            } else {
                sm->context.current_state = STATE_ERROR;
                sm->handler = handle_error;
            }
            break;
        case 3:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            sm->context.buffer[0] = '\0';
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 4:
            if (sm->context.counter < 10) {
                sm->context.counter++;
                if (sm->context.counter >= 5) {
                    sm->context.current_state = STATE_COMPLETE;
                    sm->handler = handle_complete;
                }
            } else {
                sm->context.current_state = STATE_ERROR;
                sm->handler = handle_error;
            }
            break;
        case 5:
            sm->context.current_state = STATE_READY;
            sm->handler = handle_ready;
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 6:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            sm->context.buffer[0] = '\0';
            sm->context.counter = 0;
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
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

void state_machine_process(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    if (sm->handler != NULL) {
        sm->handler(sm, evt);
    }
}

int main(void) {
    struct StateMachine sm;
    struct Event evt;
    char input[64];
    int choice;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: 1=Load, 2=Start, 3=Reset, 4=Process, 5=Cancel, 6=Finish, 7=Recover\n");
    
    while (sm.context.current_state != STATE_COMPLETE) {
        printf("\nCurrent
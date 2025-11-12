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
            if (strlen(evt->data) < sizeof(sm->context.buffer)) {
                strncpy(sm->context.buffer, evt->data, sizeof(sm->context.buffer) - 1);
                sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
                sm->context.current_state = STATE_READY;
                sm->transition = handle_ready;
            } else {
                sm->context.current_state = STATE_ERROR;
                sm->transition = handle_error;
            }
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->transition = handle_error;
            break;
    }
}

void handle_ready(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 2:
            sm->context.counter = 0;
            sm->context.current_state = STATE_PROCESSING;
            sm->transition = handle_processing;
            break;
        case 9:
            sm->context.buffer[0] = '\0';
            sm->context.current_state = STATE_IDLE;
            sm->transition = handle_idle;
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->transition = handle_error;
            break;
    }
}

void handle_processing(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 3:
            if (sm->context.counter < 100) {
                sm->context.counter++;
            }
            break;
        case 4:
            if (sm->context.counter >= 5) {
                sm->context.current_state = STATE_COMPLETE;
                sm->transition = handle_complete;
            } else {
                sm->context.current_state = STATE_ERROR;
                sm->transition = handle_error;
            }
            break;
        case 9:
            sm->context.buffer[0] = '\0';
            sm->context.counter = 0;
            sm->context.current_state = STATE_IDLE;
            sm->transition = handle_idle;
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->transition = handle_error;
            break;
    }
}

void handle_complete(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 5:
            printf("Result: %s processed %u times\n", sm->context.buffer, sm->context.counter);
            break;
        case 9:
            sm->context.buffer[0] = '\0';
            sm->context.counter = 0;
            sm->context.current_state = STATE_IDLE;
            sm->transition = handle_idle;
            break;
        default:
            sm->context.current_state = STATE_ERROR;
            sm->transition = handle_error;
            break;
    }
}

void handle_error(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 9:
            sm->context.buffer[0] = '\0';
            sm->context.counter = 0;
            sm->context.current_state = STATE_IDLE;
            sm->transition = handle_idle;
            break;
        default:
            break;
    }
}

void init_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    sm->context.buffer[0] = '\0';
    sm->transition = handle_idle;
}

int main(void) {
    struct StateMachine sm;
    struct Event evt;
    char input[64];
    int choice;
    
    init_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: 1=Load, 2=Start, 3=Process, 4=Finish, 5=Show, 9=Reset, 0=Exit\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (sm.context.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY
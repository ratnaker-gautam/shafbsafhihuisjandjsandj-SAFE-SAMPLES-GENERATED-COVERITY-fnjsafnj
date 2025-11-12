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
    if (evt == NULL || sm == NULL) return;
    
    if (evt->type == 1) {
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
    }
}

void handle_ready(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL || sm == NULL) return;
    
    if (evt->type == 2) {
        sm->context.current_state = STATE_PROCESSING;
        sm->handler = handle_processing;
    } else if (evt->type == 3) {
        sm->context.current_state = STATE_IDLE;
        sm->handler = handle_idle;
        memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL || sm == NULL) return;
    
    if (evt->type == 4) {
        if (sm->context.counter < 10) {
            size_t len = strlen(sm->context.buffer);
            if (len < sizeof(sm->context.buffer) - 2) {
                sm->context.buffer[len] = '.';
                sm->context.buffer[len + 1] = '\0';
                sm->context.counter++;
            }
        } else {
            sm->context.current_state = STATE_COMPLETE;
            sm->handler = handle_complete;
        }
    } else if (evt->type == 5) {
        sm->context.current_state = STATE_ERROR;
        sm->handler = handle_error;
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL || sm == NULL) return;
    
    if (evt->type == 6) {
        sm->context.current_state = STATE_IDLE;
        sm->handler = handle_idle;
        memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
        sm->context.counter = 0;
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (evt == NULL || sm == NULL) return;
    
    if (evt->type == 7) {
        sm->context.current_state = STATE_IDLE;
        sm->handler = handle_idle;
        memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
        sm->context.counter = 0;
    }
}

void state_machine_init(struct StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    sm->handler = handle_idle;
}

void state_machine_process(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL || sm->handler == NULL) return;
    
    sm->handler(sm, evt);
}

int main(void) {
    struct StateMachine sm;
    struct Event evt;
    char input[64];
    int choice;
    
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: 1=Load, 2=Start, 3=Reset, 4=Process, 5=Error, 6=Complete, 7=Recover, 0=Exit\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (sm.context.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | Counter: %d | Buffer: %s\n", sm.context.counter, sm.context.buffer);
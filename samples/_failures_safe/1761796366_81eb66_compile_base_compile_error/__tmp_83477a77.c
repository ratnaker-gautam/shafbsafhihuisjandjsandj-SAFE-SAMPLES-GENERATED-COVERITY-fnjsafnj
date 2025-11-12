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

void transition(struct StateMachine* sm, enum State new_state) {
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
            transition(sm, STATE_READY);
        } else {
            transition(sm, STATE_ERROR);
        }
    }
}

void handle_ready(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 2) {
        sm->context.counter = 0;
        transition(sm, STATE_PROCESSING);
    } else if (evt->type == 0) {
        sm->context.buffer[0] = '\0';
        transition(sm, STATE_IDLE);
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 3) {
        if (sm->context.counter < 1000) {
            sm->context.counter++;
        }
        if (sm->context.counter >= 5) {
            transition(sm, STATE_COMPLETE);
        }
    } else if (evt->type == 4) {
        transition(sm, STATE_ERROR);
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 0) {
        sm->context.buffer[0] = '\0';
        sm->context.counter = 0;
        transition(sm, STATE_IDLE);
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    if (evt->type == 0) {
        sm->context.buffer[0] = '\0';
        sm->context.counter = 0;
        transition(sm, STATE_IDLE);
    }
}

void init_state_machine(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    sm->context.buffer[0] = '\0';
    sm->handler = handle_idle;
}

int main(void) {
    struct StateMachine sm;
    struct Event evt;
    char input[64];
    int choice;
    
    init_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: 0=Reset, 1=Load Data, 2=Start Process, 3=Step, 4=Error\n");
    
    while (sm.context.current_state != STATE_COMPLETE) {
        printf("\nCurrent State: ");
        switch (sm.context.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | Counter: %u | Buffer: %s\n", sm.context.counter, sm.context.buffer);
        
        printf("Enter command (0-4): ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (choice < 0 || choice > 4) {
            printf("Invalid command\n");
            continue;
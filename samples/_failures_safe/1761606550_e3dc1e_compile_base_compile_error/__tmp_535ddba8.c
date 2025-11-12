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
            break;
    }
}

void handle_ready(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 2:
            if (sm->context.counter < 100) {
                sm->context.current_state = STATE_PROCESSING;
                sm->handler = handle_processing;
            } else {
                sm->context.current_state = STATE_ERROR;
                sm->handler = handle_error;
            }
            break;
        case 9:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
            break;
        default:
            break;
    }
}

void handle_processing(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 3:
            if (sm->context.counter < 100) {
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
        case 4:
            sm->context.current_state = STATE_ERROR;
            sm->handler = handle_error;
            break;
        default:
            break;
    }
}

void handle_complete(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 5:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
            sm->context.counter = 0;
            break;
        default:
            break;
    }
}

void handle_error(struct StateMachine* sm, struct Event* evt) {
    if (sm == NULL || evt == NULL) return;
    
    switch (evt->type) {
        case 6:
            sm->context.current_state = STATE_IDLE;
            sm->handler = handle_idle;
            memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
            sm->context.counter = 0;
            break;
        default:
            break;
    }
}

void init_state_machine(struct StateMachine* sm) {
    if (sm == NULL) return;
    
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    sm->handler = handle_idle;
}

int main(void) {
    struct StateMachine sm;
    struct Event events[10];
    int event_count = 0;
    
    init_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events: 1-6, 9\n");
    printf("Current state: %d\n", sm.context.current_state);
    
    while (event_count < 10) {
        int input;
        printf("Enter event type (0 to exit): ");
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        if (input == 0) {
            break;
        }
        
        if (input < 1 || input > 9) {
            printf("Invalid event type\n");
            continue;
        }
        
        events[event_count].type = input;
        
        if (input == 1) {
            printf("Enter
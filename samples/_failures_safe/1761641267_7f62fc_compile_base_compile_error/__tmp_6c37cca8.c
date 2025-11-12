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

void validate_event(struct Event *evt) {
    if (evt == NULL) {
        fprintf(stderr, "Error: Null event\n");
        exit(EXIT_FAILURE);
    }
    if (evt->type < 0 || evt->type > 5) {
        fprintf(stderr, "Error: Invalid event type\n");
        exit(EXIT_FAILURE);
    }
}

void initialize_machine(struct StateMachine *sm) {
    if (sm == NULL) {
        fprintf(stderr, "Error: Null state machine\n");
        exit(EXIT_FAILURE);
    }
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    sm->transition = handle_idle;
}

void handle_idle(struct StateMachine *sm, struct Event *evt) {
    validate_event(evt);
    if (evt->type == 1) {
        sm->context.current_state = STATE_READY;
        sm->transition = handle_ready;
        strncpy(sm->context.buffer, evt->data, sizeof(sm->context.buffer) - 1);
        sm->context.buffer[sizeof(sm->context.buffer) - 1] = '\0';
    } else if (evt->type == 4) {
        sm->context.current_state = STATE_ERROR;
        sm->transition = handle_error;
    }
}

void handle_ready(struct StateMachine *sm, struct Event *evt) {
    validate_event(evt);
    if (evt->type == 2) {
        sm->context.current_state = STATE_PROCESSING;
        sm->transition = handle_processing;
        sm->context.counter = 0;
    } else if (evt->type == 4) {
        sm->context.current_state = STATE_ERROR;
        sm->transition = handle_error;
    }
}

void handle_processing(struct StateMachine *sm, struct Event *evt) {
    validate_event(evt);
    if (evt->type == 3) {
        if (sm->context.counter < 10) {
            sm->context.counter++;
        }
        if (sm->context.counter >= 10) {
            sm->context.current_state = STATE_COMPLETE;
            sm->transition = handle_complete;
        }
    } else if (evt->type == 4) {
        sm->context.current_state = STATE_ERROR;
        sm->transition = handle_error;
    }
}

void handle_complete(struct StateMachine *sm, struct Event *evt) {
    validate_event(evt);
    if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        sm->transition = handle_idle;
        sm->context.counter = 0;
        memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    }
}

void handle_error(struct StateMachine *sm, struct Event *evt) {
    validate_event(evt);
    if (evt->type == 0) {
        sm->context.current_state = STATE_IDLE;
        sm->transition = handle_idle;
        sm->context.counter = 0;
        memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    }
}

void process_event(struct StateMachine *sm, struct Event *evt) {
    if (sm == NULL) {
        fprintf(stderr, "Error: Null state machine\n");
        exit(EXIT_FAILURE);
    }
    validate_event(evt);
    sm->transition(sm, evt);
}

int main(void) {
    struct StateMachine sm;
    initialize_machine(&sm);
    
    struct Event events[] = {
        {1, "Start processing"},
        {2, "Begin work"},
        {3, "Process step"},
        {3, "Process step"},
        {3, "Process step"},
        {3, "Process step"},
        {3, "Process step"},
        {3, "Process step"},
        {3, "Process step"},
        {3, "Process step"},
        {3, "Process step"},
        {3, "Process step"},
        {0, "Reset"},
        {1, "New data"},
        {4, "Error occurred"},
        {0, "Reset again"}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events; i++) {
        printf("Processing event %zu: type=%d, data=%s\n", 
               i
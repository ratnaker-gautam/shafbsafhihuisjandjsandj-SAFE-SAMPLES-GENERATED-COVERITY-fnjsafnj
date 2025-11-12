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

struct StateMachine {
    enum State current_state;
    uint32_t counter;
    char buffer[256];
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->type < 0 || evt->type > 4) return 0;
    return 1;
}

enum State handle_idle_state(struct StateMachine *sm, const struct Event *evt) {
    if (sm == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 0:
            return STATE_READY;
        case 1:
            return STATE_ERROR;
        default:
            return STATE_IDLE;
    }
}

enum State handle_ready_state(struct StateMachine *sm, const struct Event *evt) {
    if (sm == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 1:
            if (strlen(evt->data) < sizeof(sm->buffer)) {
                strncpy(sm->buffer, evt->data, sizeof(sm->buffer) - 1);
                sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case 2:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

enum State handle_processing_state(struct StateMachine *sm, const struct Event *evt) {
    if (sm == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 2:
            if (sm->counter < UINT32_MAX) {
                sm->counter++;
            }
            return STATE_COMPLETE;
        case 3:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

enum State handle_complete_state(struct StateMachine *sm, const struct Event *evt) {
    if (sm == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 3:
            state_machine_init(sm);
            return STATE_IDLE;
        case 4:
            return STATE_ERROR;
        default:
            return STATE_COMPLETE;
    }
}

enum State handle_error_state(struct StateMachine *sm, const struct Event *evt) {
    if (sm == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 4:
            state_machine_init(sm);
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

enum State process_event(struct StateMachine *sm, const struct Event *evt) {
    if (sm == NULL || !validate_event(evt)) {
        return STATE_ERROR;
    }
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, evt);
        case STATE_READY:
            return handle_ready_state(sm, evt);
        case STATE_PROCESSING:
            return handle_processing_state(sm, evt);
        case STATE_COMPLETE:
            return handle_complete_state(sm, evt);
        case STATE_ERROR:
            return handle_error_state(sm, evt);
        default:
            return STATE_ERROR;
    }
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Counter: %u\n", sm->counter);
    printf("Buffer: %s\n", sm->buffer);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct Event events[] = {
        {0, ""},
        {1, "Test Data"},
        {2, ""},
        {3, ""},
        {4, ""}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("Initial State:\n");
    print_state_info(&sm);
    
    for (size_t i = 0; i < num_events; i++) {
        printf("Processing Event %zu (type %d):\n", i + 1, events[i].type);
        sm.current_state = process_event(&sm, &events[i]);
        print_state_info(&sm);
    }
    
    return 0;
}
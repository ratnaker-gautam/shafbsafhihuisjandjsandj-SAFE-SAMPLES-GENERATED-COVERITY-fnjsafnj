//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: struct_heavy ; Variation: state_machine
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
    char data[64];
};

struct Context {
    int counter;
    char buffer[256];
    int buffer_len;
};

struct StateMachine {
    enum State current_state;
    struct Context context;
    int event_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->context.counter = 0;
    sm->context.buffer_len = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    sm->event_count = 0;
}

int validate_event(const struct Event *event) {
    if (event == NULL) return 0;
    if (event->type < 0 || event->type > 4) return 0;
    if (strnlen(event->data, sizeof(event->data)) >= sizeof(event->data)) return 0;
    return 1;
}

enum State handle_idle_state(struct StateMachine *sm, const struct Event *event) {
    if (event->type == 1) {
        sm->context.counter = 0;
        sm->context.buffer_len = 0;
        memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
        return STATE_READY;
    }
    return STATE_IDLE;
}

enum State handle_ready_state(struct StateMachine *sm, const struct Event *event) {
    if (event->type == 2) {
        size_t data_len = strnlen(event->data, sizeof(event->data));
        if (sm->context.buffer_len + data_len < sizeof(sm->context.buffer)) {
            memcpy(sm->context.buffer + sm->context.buffer_len, event->data, data_len);
            sm->context.buffer_len += data_len;
            sm->context.counter++;
            return STATE_PROCESSING;
        } else {
            return STATE_ERROR;
        }
    } else if (event->type == 0) {
        return STATE_IDLE;
    }
    return STATE_READY;
}

enum State handle_processing_state(struct StateMachine *sm, const struct Event *event) {
    if (event->type == 3) {
        if (sm->context.counter > 0) {
            return STATE_COMPLETE;
        } else {
            return STATE_ERROR;
        }
    } else if (event->type == 2) {
        size_t data_len = strnlen(event->data, sizeof(event->data));
        if (sm->context.buffer_len + data_len < sizeof(sm->context.buffer)) {
            memcpy(sm->context.buffer + sm->context.buffer_len, event->data, data_len);
            sm->context.buffer_len += data_len;
            sm->context.counter++;
            return STATE_PROCESSING;
        } else {
            return STATE_ERROR;
        }
    } else if (event->type == 0) {
        return STATE_IDLE;
    }
    return STATE_PROCESSING;
}

enum State handle_complete_state(struct StateMachine *sm, const struct Event *event) {
    if (event->type == 0) {
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

enum State handle_error_state(struct StateMachine *sm, const struct Event *event) {
    if (event->type == 0) {
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int state_machine_process(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || !validate_event(event)) return 0;
    
    enum State new_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            new_state = handle_idle_state(sm, event);
            break;
        case STATE_READY:
            new_state = handle_ready_state(sm, event);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state(sm, event);
            break;
        case STATE_COMPLETE:
            new_state = handle_complete_state(sm, event);
            break;
        case STATE_ERROR:
            new_state = handle_error_state(sm, event);
            break;
    }
    
    sm->current_state = new_state;
    sm->event_count++;
    return 1;
}

void print_state_machine_status(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("State: %s, Counter: %d, Buffer: %.*s, Events: %d\n",
           state_names[sm->current_state],
           sm->context.counter,
           sm->context.buffer_len,
           sm->context.buffer,
           sm->event_count);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct Event events[] = {
        {1, "Start"},
        {2, "Data1"},
        {2, "Data2"},
        {2, "Data3"},
        {3, "Process"},
        {0, "Reset"}
    };
    
    int num_events = sizeof(
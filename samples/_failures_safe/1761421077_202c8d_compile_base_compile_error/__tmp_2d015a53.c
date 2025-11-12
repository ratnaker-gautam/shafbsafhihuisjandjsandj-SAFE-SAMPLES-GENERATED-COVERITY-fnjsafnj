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
    struct Event pending_event;
    int has_event;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    sm->has_event = 0;
    memset(&sm->pending_event, 0, sizeof(sm->pending_event));
}

int state_machine_queue_event(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return 0;
    if (sm->has_event) return 0;
    if (strlen(event->data) >= sizeof(event->data)) return 0;
    sm->pending_event = *event;
    sm->has_event = 1;
    return 1;
}

enum State state_machine_handle_idle(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return STATE_IDLE;
    if (event->type == 1) {
        if (strlen(event->data) < sizeof(sm->context.buffer)) {
            strncpy(sm->context.buffer, event->data, sizeof(sm->context.buffer)-1);
            sm->context.buffer[sizeof(sm->context.buffer)-1] = '\0';
            return STATE_READY;
        }
        return STATE_ERROR;
    }
    return STATE_IDLE;
}

enum State state_machine_handle_ready(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return STATE_READY;
    if (event->type == 2) {
        sm->context.counter = 0;
        for (size_t i = 0; i < strlen(sm->context.buffer); i++) {
            if (sm->context.counter < UINT32_MAX) {
                sm->context.counter++;
            }
        }
        return STATE_PROCESSING;
    } else if (event->type == 3) {
        return STATE_IDLE;
    }
    return STATE_READY;
}

enum State state_machine_handle_processing(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return STATE_PROCESSING;
    if (event->type == 4) {
        if (sm->context.counter > 0) {
            if (sm->context.counter <= UINT32_MAX - 10) {
                sm->context.counter += 10;
            }
        }
        return STATE_COMPLETE;
    } else if (event->type == 5) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

enum State state_machine_handle_complete(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return STATE_COMPLETE;
    if (event->type == 6) {
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

enum State state_machine_handle_error(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || event == NULL) return STATE_ERROR;
    if (event->type == 7) {
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int state_machine_process(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (!sm->has_event) return 0;
    
    enum State new_state = sm->context.current_state;
    struct Event *event = &sm->pending_event;
    
    switch (sm->context.current_state) {
        case STATE_IDLE:
            new_state = state_machine_handle_idle(sm, event);
            break;
        case STATE_READY:
            new_state = state_machine_handle_ready(sm, event);
            break;
        case STATE_PROCESSING:
            new_state = state_machine_handle_processing(sm, event);
            break;
        case STATE_COMPLETE:
            new_state = state_machine_handle_complete(sm, event);
            break;
        case STATE_ERROR:
            new_state = state_machine_handle_error(sm, event);
            break;
        default:
            new_state = STATE_ERROR;
            break;
    }
    
    sm->context.current_state = new_state;
    sm->has_event = 0;
    return 1;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    printf("Current State: %s\n", state_names[sm->context.current_state]);
    printf("Counter: %u\n", sm->context.counter);
    printf("Buffer: %s\n", sm->context.buffer);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm
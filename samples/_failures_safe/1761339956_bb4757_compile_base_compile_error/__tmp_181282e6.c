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
    int event_count;
};

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
    sm->event_count = 0;
}

int validate_event(const struct Event *event) {
    if (event == NULL) return 0;
    if (event->type < 0 || event->type > 4) return 0;
    return 1;
}

enum State handle_idle_state(struct Context *ctx, const struct Event *event) {
    if (ctx == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->type) {
        case 0:
            ctx->counter = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            return STATE_READY;
        case 1:
            return STATE_ERROR;
        default:
            return STATE_IDLE;
    }
}

enum State handle_ready_state(struct Context *ctx, const struct Event *event) {
    if (ctx == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->type) {
        case 1:
            if (strlen(event->data) < sizeof(ctx->buffer)) {
                strncpy(ctx->buffer, event->data, sizeof(ctx->buffer) - 1);
                ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case 2:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

enum State handle_processing_state(struct Context *ctx, const struct Event *event) {
    if (ctx == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->type) {
        case 2:
            if (ctx->counter < 100) {
                ctx->counter++;
                return STATE_PROCESSING;
            }
            return STATE_COMPLETE;
        case 3:
            return STATE_COMPLETE;
        case 4:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

enum State handle_complete_state(struct Context *ctx, const struct Event *event) {
    if (ctx == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->type) {
        case 0:
            return STATE_IDLE;
        case 4:
            return STATE_ERROR;
        default:
            return STATE_COMPLETE;
    }
}

enum State handle_error_state(struct Context *ctx, const struct Event *event) {
    if (ctx == NULL || event == NULL) return STATE_ERROR;
    
    switch (event->type) {
        case 0:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

enum State process_event(struct StateMachine *sm, const struct Event *event) {
    if (sm == NULL || !validate_event(event)) return STATE_ERROR;
    
    enum State next_state = sm->context.current_state;
    
    switch (sm->context.current_state) {
        case STATE_IDLE:
            next_state = handle_idle_state(&sm->context, event);
            break;
        case STATE_READY:
            next_state = handle_ready_state(&sm->context, event);
            break;
        case STATE_PROCESSING:
            next_state = handle_processing_state(&sm->context, event);
            break;
        case STATE_COMPLETE:
            next_state = handle_complete_state(&sm->context, event);
            break;
        case STATE_ERROR:
            next_state = handle_error_state(&sm->context, event);
            break;
        default:
            next_state = STATE_ERROR;
            break;
    }
    
    sm->context.current_state = next_state;
    sm->event_count++;
    return next_state;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->context.current_state]);
    printf("Counter: %d\n", sm->context.counter);
    printf("Buffer: %s\n", sm->context.buffer);
    printf("Events Processed: %d\n", sm->event_count);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm;
    initialize_state_machine(&sm);
    
    struct Event events[] = {
        {0, ""},
        {1, "Test Data"},
        {2, ""},
        {2, ""},
        {3, ""},
        {0, ""},
        {4, ""},
        {0, ""}
    };
    
    int num_events = sizeof(events) / sizeof(events[0]);
    
    printf("Initial State:\n");
    print_state_info(&sm);
    
    for (int i = 0; i < num_events; i++) {
        printf("Processing Event
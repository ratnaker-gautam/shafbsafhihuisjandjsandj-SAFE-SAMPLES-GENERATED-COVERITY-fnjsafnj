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

struct Transition {
    enum State from_state;
    int event_type;
    enum State to_state;
    int (*action)(struct Context*, struct Event*);
};

int action_idle_to_ready(struct Context* ctx, struct Event* evt) {
    if (ctx == NULL || evt == NULL) return -1;
    if (strlen(evt->data) >= sizeof(ctx->buffer)) return -1;
    strncpy(ctx->buffer, evt->data, sizeof(ctx->buffer) - 1);
    ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
    ctx->counter = 0;
    return 0;
}

int action_ready_to_processing(struct Context* ctx, struct Event* evt) {
    if (ctx == NULL || evt == NULL) return -1;
    if (ctx->counter < 0) return -1;
    ctx->counter += 1;
    return 0;
}

int action_processing_to_complete(struct Context* ctx, struct Event* evt) {
    if (ctx == NULL || evt == NULL) return -1;
    if (ctx->counter > 100) return -1;
    ctx->counter *= 2;
    return 0;
}

int action_error(struct Context* ctx, struct Event* evt) {
    if (ctx == NULL || evt == NULL) return -1;
    ctx->counter = -1;
    ctx->buffer[0] = '\0';
    return 0;
}

int action_noop(struct Context* ctx, struct Event* evt) {
    return 0;
}

struct Transition transitions[] = {
    {STATE_IDLE, 1, STATE_READY, action_idle_to_ready},
    {STATE_READY, 2, STATE_PROCESSING, action_ready_to_processing},
    {STATE_PROCESSING, 3, STATE_COMPLETE, action_processing_to_complete},
    {STATE_READY, 4, STATE_ERROR, action_error},
    {STATE_PROCESSING, 4, STATE_ERROR, action_error},
    {STATE_COMPLETE, 0, STATE_IDLE, action_noop},
    {STATE_ERROR, 0, STATE_IDLE, action_noop}
};

enum State handle_event(struct Context* ctx, struct Event* evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].from_state == ctx->current_state && 
            transitions[i].event_type == evt->type) {
            if (transitions[i].action(ctx, evt) == 0) {
                ctx->current_state = transitions[i].to_state;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            return ctx->current_state;
        }
    }
    return ctx->current_state;
}

const char* state_to_string(enum State s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.counter = 0;
    ctx.buffer[0] = '\0';
    
    struct Event events[] = {
        {1, "Initial data"},
        {2, ""},
        {3, ""},
        {0, ""},
        {1, "New data"},
        {4, ""},
        {0, ""}
    };
    
    printf("State Machine Simulation\n");
    printf("Initial state: %s\n", state_to_string(ctx.current_state));
    
    for (size_t i = 0; i < sizeof(events) / sizeof(events[0]); i++) {
        if (i >= sizeof(events) / sizeof(events[0])) break;
        
        printf("Processing event type %d with data: %s\n", 
               events[i].type, events[i].data);
        
        enum State new_state = handle_event(&ctx, &events[i]);
        
        printf("New state: %s, Counter: %d, Buffer: %s\n", 
               state_to_string(new_state), ctx.counter, ctx.buffer);
        
        if (new_state == STATE_ERROR) {
            printf("Error state detected, resetting...\n");
        }
    }
    
    return 0;
}
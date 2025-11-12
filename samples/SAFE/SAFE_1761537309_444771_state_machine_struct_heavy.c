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
    char data[64];
};

struct Context {
    enum State current_state;
    uint32_t counter;
    char buffer[256];
    size_t buffer_len;
};

struct StateTransition {
    enum State from_state;
    int event_type;
    enum State to_state;
    int (*action)(struct Context*);
};

int action_idle_to_ready(struct Context* ctx) {
    if (ctx == NULL) return -1;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    return 0;
}

int action_ready_to_processing(struct Context* ctx) {
    if (ctx == NULL) return -1;
    if (ctx->buffer_len >= sizeof(ctx->buffer)) return -1;
    ctx->counter++;
    return 0;
}

int action_processing_to_complete(struct Context* ctx) {
    if (ctx == NULL) return -1;
    if (ctx->counter == 0) return -1;
    return 0;
}

int action_error_recovery(struct Context* ctx) {
    if (ctx == NULL) return -1;
    ctx->counter = 0;
    ctx->buffer_len = 0;
    return 0;
}

struct StateTransition transitions[] = {
    {STATE_IDLE, 1, STATE_READY, action_idle_to_ready},
    {STATE_READY, 2, STATE_PROCESSING, action_ready_to_processing},
    {STATE_PROCESSING, 3, STATE_COMPLETE, action_processing_to_complete},
    {STATE_PROCESSING, 4, STATE_ERROR, NULL},
    {STATE_ERROR, 5, STATE_IDLE, action_error_recovery},
    {STATE_COMPLETE, 1, STATE_IDLE, NULL}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct Context* ctx, const struct Event* event) {
    if (ctx == NULL || event == NULL) return -1;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == ctx->current_state && 
            transitions[i].event_type == event->type) {
            
            if (transitions[i].action != NULL) {
                if (transitions[i].action(ctx) != 0) {
                    return -1;
                }
            }
            
            ctx->current_state = transitions[i].to_state;
            return 0;
        }
    }
    
    return -1;
}

void initialize_context(struct Context* ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
}

const char* state_to_string(enum State state) {
    switch (state) {
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
    initialize_context(&ctx);
    
    struct Event events[] = {
        {1, "Initialize"},
        {2, "Start processing"},
        {3, "Complete"},
        {1, "Reset"},
        {2, "Start again"},
        {4, "Error occurred"},
        {5, "Recover"},
        {2, "Restart"},
        {3, "Finish"}
    };
    
    const size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("Initial state: %s\n", state_to_string(ctx.current_state));
    
    for (size_t i = 0; i < num_events; i++) {
        printf("Processing event %d: %s\n", events[i].type, events[i].data);
        
        if (process_event(&ctx, &events[i]) != 0) {
            printf("Error processing event %zu\n", i);
            break;
        }
        
        printf("New state: %s (counter: %u)\n", state_to_string(ctx.current_state), ctx.counter);
        
        if (ctx.current_state == STATE_ERROR) {
            printf("Error state detected, waiting for recovery...\n");
        }
    }
    
    printf("Final state: %s\n", state_to_string(ctx.current_state));
    
    return 0;
}
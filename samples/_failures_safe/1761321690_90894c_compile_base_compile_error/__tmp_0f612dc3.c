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

struct Context {
    enum State current_state;
    uint32_t data_value;
    uint32_t counter;
    char buffer[64];
};

struct Event {
    int type;
    uint32_t value;
};

void state_machine_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->data_value = 0;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int handle_idle_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 1) {
        ctx->current_state = STATE_READY;
        ctx->data_value = evt->value;
        return 1;
    }
    return 0;
}

int handle_ready_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 2) {
        if (evt->value > 1000) {
            ctx->current_state = STATE_ERROR;
            return 1;
        }
        ctx->current_state = STATE_PROCESSING;
        ctx->counter = evt->value;
        return 1;
    } else if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int handle_processing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 3) {
        if (ctx->counter == 0) {
            ctx->current_state = STATE_ERROR;
            return 1;
        }
        
        if (ctx->data_value > UINT32_MAX / ctx->counter) {
            ctx->current_state = STATE_ERROR;
            return 1;
        }
        
        uint32_t result = ctx->data_value * ctx->counter;
        if (snprintf(ctx->buffer, sizeof(ctx->buffer), "Result: %u", result) >= (int)sizeof(ctx->buffer)) {
            ctx->current_state = STATE_ERROR;
            return 1;
        }
        
        ctx->current_state = STATE_COMPLETE;
        return 1;
    } else if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int handle_complete_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 1;
    } else if (evt->type == 4) {
        printf("%s\n", ctx->buffer);
        return 1;
    }
    return 0;
}

int handle_error_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 1;
    } else if (evt->type == 5) {
        printf("Error state: Reset required\n");
        return 1;
    }
    return 0;
}

int process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle_state(ctx, evt);
        case STATE_READY:
            return handle_ready_state(ctx, evt);
        case STATE_PROCESSING:
            return handle_processing_state(ctx, evt);
        case STATE_COMPLETE:
            return handle_complete_state(ctx, evt);
        case STATE_ERROR:
            return handle_error_state(ctx, evt);
        default:
            return 0;
    }
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
    state_machine_init(&ctx);
    
    struct Event events[] = {
        {1, 42},
        {2, 5},
        {3, 0},
        {4, 0},
        {0, 0},
        {1, 100},
        {2, 10},
        {3, 0},
        {4, 0},
        {5, 0}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("Starting state machine simulation\n");
    
    for (size_t i = 0; i < num_events; i++) {
        printf("Current state: %s, Processing event type: %d\n", 
               state
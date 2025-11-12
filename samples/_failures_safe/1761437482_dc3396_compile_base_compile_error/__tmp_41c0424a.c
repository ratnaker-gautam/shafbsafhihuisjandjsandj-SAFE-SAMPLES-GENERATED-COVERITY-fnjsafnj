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
    uint32_t param;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->data_value = 0;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->type < 0 || evt->type > 4) return 0;
    return 1;
}

int handle_idle_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 1) {
        ctx->current_state = STATE_READY;
        ctx->data_value = evt->param;
        return 1;
    }
    return 0;
}

int handle_ready_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 2) {
        if (ctx->data_value > 1000) {
            ctx->current_state = STATE_ERROR;
            return 1;
        }
        ctx->current_state = STATE_PROCESSING;
        ctx->counter = 0;
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
        if (ctx->counter >= 10) {
            ctx->current_state = STATE_COMPLETE;
            snprintf(ctx->buffer, sizeof(ctx->buffer), "Processed: %u", ctx->data_value);
            return 1;
        }
        if (ctx->data_value > UINT32_MAX - ctx->counter) {
            ctx->current_state = STATE_ERROR;
            return 1;
        }
        ctx->data_value += ctx->counter;
        ctx->counter++;
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
    }
    return 0;
}

int handle_error_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 0) {
        ctx->current_state = STATE_IDLE;
        return 1;
    }
    return 0;
}

int process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return 0;
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

void print_state_info(const struct Context *ctx) {
    if (ctx == NULL) return;
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    printf("State: %s, Data: %u, Counter: %u", 
           state_names[ctx->current_state], 
           ctx->data_value, 
           ctx->counter);
    if (strlen(ctx->buffer) > 0) {
        printf(", Buffer: %s", ctx->buffer);
    }
    printf("\n");
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    struct Event events[] = {
        {1, 50},
        {2, 0},
        {3, 0},
        {3, 0},
        {3, 0},
        {3, 0},
        {3, 0},
        {3, 0},
        {3, 0},
        {3, 0},
        {3, 0},
        {3, 0},
        {0, 0}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("Initial state: ");
    print_state_info(&ctx);
    
    for (size_t i = 0; i < num_events; i++) {
        printf("Processing event type
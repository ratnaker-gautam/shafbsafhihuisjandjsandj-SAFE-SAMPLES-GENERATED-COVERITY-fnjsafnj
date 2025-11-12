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
    uint8_t retry_count;
    char buffer[64];
    size_t buffer_len;
};

struct Event {
    int type;
    uint32_t param;
};

void context_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->data_value = 0;
    ctx->retry_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->type < 0 || evt->type > 5) return 0;
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
        ctx->current_state = STATE_PROCESSING;
        if (ctx->buffer_len < sizeof(ctx->buffer) - 1) {
            int written = snprintf(ctx->buffer + ctx->buffer_len, 
                                 sizeof(ctx->buffer) - ctx->buffer_len,
                                 "Processing:%u", evt->param);
            if (written > 0 && (size_t)written < sizeof(ctx->buffer) - ctx->buffer_len) {
                ctx->buffer_len += written;
            }
        }
        return 1;
    }
    return 0;
}

int handle_processing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 3) {
        ctx->current_state = STATE_COMPLETE;
        if (ctx->buffer_len < sizeof(ctx->buffer) - 1) {
            int written = snprintf(ctx->buffer + ctx->buffer_len,
                                 sizeof(ctx->buffer) - ctx->buffer_len,
                                 "|Complete:%u", evt->param);
            if (written > 0 && (size_t)written < sizeof(ctx->buffer) - ctx->buffer_len) {
                ctx->buffer_len += written;
            }
        }
        return 1;
    } else if (evt->type == 4) {
        ctx->current_state = STATE_ERROR;
        ctx->retry_count++;
        return 1;
    }
    return 0;
}

int handle_complete_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 0) {
        context_init(ctx);
        return 1;
    }
    return 0;
}

int handle_error_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 0) {
        context_init(ctx);
        return 1;
    } else if (evt->type == 5 && ctx->retry_count < 3) {
        ctx->current_state = STATE_READY;
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
    
    printf("State: %s, Data: %u, Retries: %u, Buffer: %s\n",
           state_names[ctx->current_state],
           ctx->data_value,
           ctx->retry_count,
           ctx->buffer);
}

int main(void) {
    struct Context ctx;
    context_init(&ctx);
    
    struct Event events[] = {
        {1, 100},
        {2, 200},
        {3, 300},
        {0, 0},
        {1, 400},
        {2, 500},
        {4, 0},
        {5, 0},
        {2, 600},
        {3, 700},
        {0, 0}
    };
    
    size_t num_
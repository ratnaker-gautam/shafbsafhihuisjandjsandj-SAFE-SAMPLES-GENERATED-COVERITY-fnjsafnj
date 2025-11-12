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
    uint32_t counter;
    char buffer[64];
    size_t buffer_len;
};

struct Event {
    int type;
    char data[32];
    size_t data_len;
};

void context_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->data_len >= sizeof(evt->data)) return 0;
    return 1;
}

enum State handle_idle_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 1) {
        if (evt->data_len > 0 && evt->data_len < sizeof(ctx->buffer)) {
            memcpy(ctx->buffer, evt->data, evt->data_len);
            ctx->buffer_len = evt->data_len;
            return STATE_READY;
        }
    }
    return STATE_IDLE;
}

enum State handle_ready_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 2) {
        if (ctx->counter < 1000) {
            ctx->counter++;
            return STATE_PROCESSING;
        }
    } else if (evt->type == 3) {
        return STATE_IDLE;
    }
    return STATE_READY;
}

enum State handle_processing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 4) {
        if (ctx->buffer_len > 0) {
            for (size_t i = 0; i < ctx->buffer_len; i++) {
                if (ctx->buffer[i] != 0) {
                    ctx->buffer[i] = (char)((ctx->buffer[i] + 1) % 128);
                }
            }
            return STATE_COMPLETE;
        }
    } else if (evt->type == 5) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

enum State handle_complete_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 6) {
        printf("Processing complete. Counter: %u, Buffer: ", ctx->counter);
        for (size_t i = 0; i < ctx->buffer_len; i++) {
            printf("%c", ctx->buffer[i]);
        }
        printf("\n");
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

enum State handle_error_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 7) {
        context_init(ctx);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

enum State process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) {
        return STATE_ERROR;
    }
    
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
            return STATE_ERROR;
    }
}

int main(void) {
    struct Context ctx;
    context_init(&ctx);
    
    struct Event events[] = {
        {1, "Hello", 5},
        {2, "", 0},
        {4, "", 0},
        {6, "", 0},
        {1, "Test", 4},
        {2, "", 0},
        {5, "", 0},
        {7, "", 0}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events; i++) {
        enum State new_state = process_event(&ctx, &events[i]);
        if (new_state == STATE_ERROR && ctx.current_state != STATE_ERROR) {
            printf("Error processing event %zu\n", i);
        }
        ctx.current_state = new_state;
        
        if (ctx.current_state == STATE_COMPLETE) {
            printf("State: COMPLETE\n");
        } else if (ctx.current_state == STATE_ERROR) {
            printf("State: ERROR\n");
        }
    }
    
    return 0;
}
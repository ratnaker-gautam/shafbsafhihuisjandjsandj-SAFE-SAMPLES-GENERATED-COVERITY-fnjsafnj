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

void state_machine_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->data_value = 0;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->type < 0 || evt->type > 6) return 0;
    return 1;
}

enum State handle_idle_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 1:
            if (evt->param <= 1000) {
                ctx->data_value = evt->param;
                return STATE_READY;
            }
            break;
        case 2:
            return STATE_ERROR;
        default:
            break;
    }
    return STATE_IDLE;
}

enum State handle_ready_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 3:
            if (ctx->data_value > 0 && ctx->data_value <= 1000) {
                if (ctx->data_value > UINT32_MAX / 2) return STATE_ERROR;
                ctx->counter = ctx->data_value * 2;
                return STATE_PROCESSING;
            }
            break;
        case 4:
            return STATE_IDLE;
        default:
            break;
    }
    return STATE_READY;
}

enum State handle_processing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 5:
            if (ctx->counter > 0) {
                int written = snprintf(ctx->buffer, sizeof(ctx->buffer) - 1, 
                                      "Processed: %u", ctx->counter);
                if (written < 0 || (size_t)written >= sizeof(ctx->buffer)) {
                    return STATE_ERROR;
                }
                ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
                return STATE_COMPLETE;
            }
            break;
        case 6:
            return STATE_READY;
        default:
            break;
    }
    return STATE_PROCESSING;
}

enum State handle_complete_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    switch (evt->type) {
        case 0:
            state_machine_init(ctx);
            return STATE_IDLE;
        case 2:
            return STATE_ERROR;
        default:
            break;
    }
    return STATE_COMPLETE;
}

enum State handle_error_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return STATE_ERROR;
    
    if (evt->type == 0) {
        state_machine_init(ctx);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) {
        return 0;
    }
    
    enum State new_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            new_state = handle_idle_state(ctx, evt);
            break;
        case STATE_READY:
            new_state = handle_ready_state(ctx, evt);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state(ctx, evt);
            break;
        case STATE_COMPLETE:
            new_state = handle_complete_state(ctx, evt);
            break;
        case STATE_ERROR:
            new_state = handle_error_state(ctx, evt);
            break;
        default:
            new_state = STATE_ERROR;
            break;
    }
    
    ctx->current_state = new_state;
    return 1;
}

void print_state_info(const struct Context *ctx) {
    if (ctx == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    int state_index = ctx->current_state;
    if (state_index < 0 || state_index >= 5) {
        printf("Invalid state\n");
        return;
    }
    
    printf("State: %s, Data: %u, Counter: %u", 
           state_names[state_index], ctx->data_value, ctx->counter);
    
    if (ctx->current_state == STATE_COMPLETE && ctx->buffer[0] != '\0') {
        printf(", Buffer: %s", ctx->buffer);
    }
    printf("\n");
}

int main(void) {
    struct Context machine;
    struct Event event;
    int input_type;
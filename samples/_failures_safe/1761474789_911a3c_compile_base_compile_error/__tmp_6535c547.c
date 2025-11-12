//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_len;
    int data_count;
    int max_data;
};

struct Event {
    int type;
    char data[64];
    size_t data_len;
};

void context_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
    ctx->max_data = 10;
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->type < 0 || evt->type > 5) return 0;
    if (evt->data_len >= sizeof(evt->data)) return 0;
    return 1;
}

int handle_idle(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 1) {
        if (evt->data_len > 0 && evt->data_len < sizeof(ctx->buffer)) {
            memcpy(ctx->buffer, evt->data, evt->data_len);
            ctx->buffer_len = evt->data_len;
            ctx->current_state = READING;
            return 1;
        }
    }
    return 0;
}

int handle_reading(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 2) {
        if (ctx->buffer_len + evt->data_len < sizeof(ctx->buffer)) {
            memcpy(ctx->buffer + ctx->buffer_len, evt->data, evt->data_len);
            ctx->buffer_len += evt->data_len;
            ctx->current_state = PROCESSING;
            return 1;
        }
    } else if (evt->type == 4) {
        ctx->current_state = ERROR;
        return 1;
    }
    return 0;
}

int handle_processing(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 3) {
        if (ctx->data_count < ctx->max_data) {
            for (size_t i = 0; i < ctx->buffer_len; i++) {
                if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                    ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                }
            }
            ctx->data_count++;
            ctx->current_state = WRITING;
            return 1;
        } else {
            ctx->current_state = DONE;
            return 1;
        }
    } else if (evt->type == 4) {
        ctx->current_state = ERROR;
        return 1;
    }
    return 0;
}

int handle_writing(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 0) {
        printf("Processed data: %.*s\n", (int)ctx->buffer_len, ctx->buffer);
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_len = 0;
        if (ctx->data_count >= ctx->max_data) {
            ctx->current_state = DONE;
        } else {
            ctx->current_state = IDLE;
        }
        return 1;
    } else if (evt->type == 4) {
        ctx->current_state = ERROR;
        return 1;
    }
    return 0;
}

int handle_error(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    
    if (evt->type == 5) {
        printf("Error state cleared\n");
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_len = 0;
        ctx->current_state = IDLE;
        return 1;
    }
    return 0;
}

int state_machine_step(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return 0;
    
    switch (ctx->current_state) {
        case IDLE:
            return handle_idle(ctx, evt);
        case READING:
            return handle_reading(ctx, evt);
        case PROCESSING:
            return handle_processing(ctx, evt);
        case WRITING:
            return handle_writing(ctx, evt);
        case ERROR:
            return handle_error(ctx, evt);
        case DONE:
            return 0;
        default:
            return 0;
    }
}

const char* state_to_string(enum State state) {
    switch (state) {
        case IDLE: return "IDLE";
        case READING: return "READING";
        case PROCESSING: return "PROCESSING";
        case WRITING: return "WRITING";
        case ERROR: return "ERROR";
        case DONE: return "D
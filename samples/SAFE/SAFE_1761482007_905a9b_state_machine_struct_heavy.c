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
};

void state_machine_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
    ctx->max_data = 10;
}

enum State handle_idle(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return ERROR;
    if (evt->type == 1) {
        printf("Starting data processing\n");
        return READING;
    }
    return IDLE;
}

enum State handle_reading(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return ERROR;
    if (evt->type == 2) {
        size_t data_len = strlen(evt->data);
        if (data_len > 0 && data_len < sizeof(ctx->buffer) - ctx->buffer_len) {
            memcpy(ctx->buffer + ctx->buffer_len, evt->data, data_len);
            ctx->buffer_len += data_len;
            ctx->data_count++;
            printf("Read data: %s\n", evt->data);
            if (ctx->data_count >= ctx->max_data) {
                return PROCESSING;
            }
            return READING;
        } else {
            return ERROR;
        }
    }
    return READING;
}

enum State handle_processing(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return ERROR;
    if (evt->type == 3) {
        printf("Processing %zu bytes of data\n", ctx->buffer_len);
        for (size_t i = 0; i < ctx->buffer_len; i++) {
            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
            }
        }
        return WRITING;
    }
    return PROCESSING;
}

enum State handle_writing(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return ERROR;
    if (evt->type == 4) {
        printf("Processed data: %s\n", ctx->buffer);
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_len = 0;
        ctx->data_count = 0;
        return DONE;
    }
    return WRITING;
}

enum State handle_error(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return ERROR;
    if (evt->type == 5) {
        printf("Resetting from error state\n");
        state_machine_init(ctx);
        return IDLE;
    }
    return ERROR;
}

enum State handle_done(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return ERROR;
    if (evt->type == 6) {
        printf("Restarting state machine\n");
        state_machine_init(ctx);
        return IDLE;
    }
    return DONE;
}

enum State process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return ERROR;
    switch (ctx->current_state) {
        case IDLE: return handle_idle(ctx, evt);
        case READING: return handle_reading(ctx, evt);
        case PROCESSING: return handle_processing(ctx, evt);
        case WRITING: return handle_writing(ctx, evt);
        case ERROR: return handle_error(ctx, evt);
        case DONE: return handle_done(ctx, evt);
        default: return ERROR;
    }
}

int main(void) {
    struct Context ctx;
    state_machine_init(&ctx);
    
    struct Event events[] = {
        {1, ""},
        {2, "hello"},
        {2, "world"},
        {2, "test"},
        {2, "data"},
        {2, "processing"},
        {2, "state"},
        {2, "machine"},
        {2, "example"},
        {2, "program"},
        {2, "final"},
        {3, ""},
        {4, ""},
        {6, ""}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events; i++) {
        if (ctx.current_state == ERROR) break;
        enum State new_state = process_event(&ctx, &events[i]);
        ctx.current_state = new_state;
    }
    
    return 0;
}
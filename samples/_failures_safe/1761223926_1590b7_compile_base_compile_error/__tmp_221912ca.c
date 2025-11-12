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
    int processed_count;
    int error_code;
};

struct Event {
    int type;
    char data[128];
    size_t data_len;
};

void state_machine_init(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->error_code = 0;
}

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->type < 0 || evt->type > 5) return 0;
    if (evt->data_len >= sizeof(evt->data)) return 0;
    return 1;
}

int handle_idle_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 1) {
        ctx->current_state = READING;
        return 1;
    }
    return 0;
}

int handle_reading_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 2) {
        if (evt->data_len > 0 && evt->data_len < sizeof(ctx->buffer)) {
            memcpy(ctx->buffer, evt->data, evt->data_len);
            ctx->buffer_len = evt->data_len;
            ctx->current_state = PROCESSING;
            return 1;
        } else {
            ctx->current_state = ERROR;
            ctx->error_code = 1;
            return 0;
        }
    }
    return 0;
}

int handle_processing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 3) {
        if (ctx->buffer_len > 0) {
            for (size_t i = 0; i < ctx->buffer_len; i++) {
                if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                    ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                }
            }
            ctx->processed_count++;
            ctx->current_state = WRITING;
            return 1;
        } else {
            ctx->current_state = ERROR;
            ctx->error_code = 2;
            return 0;
        }
    }
    return 0;
}

int handle_writing_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 4) {
        if (ctx->buffer_len > 0) {
            printf("Processed: %.*s\n", (int)ctx->buffer_len, ctx->buffer);
            ctx->current_state = DONE;
            return 1;
        } else {
            ctx->current_state = ERROR;
            ctx->error_code = 3;
            return 0;
        }
    }
    return 0;
}

int handle_error_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 0) {
        state_machine_init(ctx);
        return 1;
    }
    return 0;
}

int handle_done_state(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || evt == NULL) return 0;
    if (evt->type == 0) {
        state_machine_init(ctx);
        return 1;
    }
    return 0;
}

int process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return 0;
    
    switch (ctx->current_state) {
        case IDLE:
            return handle_idle_state(ctx, evt);
        case READING:
            return handle_reading_state(ctx, evt);
        case PROCESSING:
            return handle_processing_state(ctx, evt);
        case WRITING:
            return handle_writing_state(ctx, evt);
        case ERROR:
            return handle_error_state(ctx, evt);
        case DONE:
            return handle_done_state(ctx, evt);
        default:
            return 0;
    }
}

int main(void) {
    struct Context ctx;
    state_machine_init(&ctx);
    
    struct Event events[] = {
        {1, "", 0},
        {2, "hello world", 11},
        {3, "", 0},
        {4, "", 0},
        {0, "", 0}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events; i++) {
        if (!process_event(&ctx, &events[i])) {
            if (ctx.current_state == ERROR) {
                printf("Error occurred: %d\n", ctx.error_code);
            }
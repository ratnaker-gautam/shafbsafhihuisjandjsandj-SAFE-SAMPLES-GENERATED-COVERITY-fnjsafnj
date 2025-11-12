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

int validate_event(const struct Event *evt) {
    if (evt == NULL) return 0;
    if (evt->type < 0 || evt->type > 5) return 0;
    return 1;
}

enum State handle_idle(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return ERROR;
    if (evt->type == 1) {
        if (strlen(evt->data) < sizeof(ctx->buffer)) {
            strncpy(ctx->buffer, evt->data, sizeof(ctx->buffer)-1);
            ctx->buffer[sizeof(ctx->buffer)-1] = '\0';
            ctx->buffer_len = strlen(ctx->buffer);
            return READING;
        }
    }
    return IDLE;
}

enum State handle_reading(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return ERROR;
    if (evt->type == 2) {
        if (ctx->buffer_len > 0) {
            return PROCESSING;
        }
    } else if (evt->type == 1) {
        size_t new_len = ctx->buffer_len + strlen(evt->data);
        if (new_len < sizeof(ctx->buffer)) {
            strncat(ctx->buffer, evt->data, sizeof(ctx->buffer)-ctx->buffer_len-1);
            ctx->buffer_len = strlen(ctx->buffer);
            return READING;
        }
    }
    return READING;
}

enum State handle_processing(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return ERROR;
    if (evt->type == 3) {
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
    if (ctx == NULL || !validate_event(evt)) return ERROR;
    if (evt->type == 4) {
        if (ctx->buffer_len > 0) {
            printf("Processed: %s\n", ctx->buffer);
            ctx->data_count++;
            if (ctx->data_count >= ctx->max_data) {
                return DONE;
            }
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            ctx->buffer_len = 0;
            return IDLE;
        }
    }
    return WRITING;
}

enum State handle_error(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return ERROR;
    if (evt->type == 0) {
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_len = 0;
        return IDLE;
    }
    return ERROR;
}

void process_event(struct Context *ctx, const struct Event *evt) {
    if (ctx == NULL || !validate_event(evt)) return;
    
    enum State next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case IDLE:
            next_state = handle_idle(ctx, evt);
            break;
        case READING:
            next_state = handle_reading(ctx, evt);
            break;
        case PROCESSING:
            next_state = handle_processing(ctx, evt);
            break;
        case WRITING:
            next_state = handle_writing(ctx, evt);
            break;
        case ERROR:
            next_state = handle_error(ctx, evt);
            break;
        case DONE:
            break;
    }
    
    ctx->current_state = next_state;
}

int main(void) {
    struct Context ctx;
    state_machine_init(&ctx);
    
    struct Event events[] = {
        {1, "hello"},
        {1, " world"},
        {2, ""},
        {3, ""},
        {4, ""},
        {1, "test"},
        {2, ""},
        {3, ""},
        {4, ""},
        {1, "final"},
        {2, ""},
        {3, ""},
        {4, ""}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < num_events && ctx.current_state != DONE; i++) {
        process_event(&ctx, &events[i]);
    }
    
    if (ctx.current_state == DONE) {
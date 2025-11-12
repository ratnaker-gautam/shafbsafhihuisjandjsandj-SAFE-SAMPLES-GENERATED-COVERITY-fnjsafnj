//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int error_code;
    int data_count;
};

static int validate_buffer_size(size_t len) {
    return len < sizeof(((struct context *)0)->buffer);
}

static void handle_idle(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_START:
            ctx->current_state = STATE_READING;
            ctx->data_count = 0;
            ctx->error_code = 0;
            break;
        case EVENT_RESET:
            break;
        default:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 1;
            break;
    }
}

static void handle_reading(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_DATA_READY:
            if (ctx->data_count < 10) {
                size_t data_len = (size_t)(ctx->data_count + 5);
                if (validate_buffer_size(data_len)) {
                    memset(ctx->buffer, 'A' + ctx->data_count, data_len);
                    ctx->buffer_len = data_len;
                    ctx->current_state = STATE_PROCESSING;
                    ctx->data_count++;
                } else {
                    ctx->current_state = STATE_ERROR;
                    ctx->error_code = 2;
                }
            } else {
                ctx->current_state = STATE_DONE;
            }
            break;
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            break;
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 3;
            break;
        default:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 4;
            break;
    }
}

static void handle_processing(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_PROCESS_COMPLETE:
            for (size_t i = 0; i < ctx->buffer_len && i < sizeof(ctx->buffer); i++) {
                if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                    ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                }
            }
            ctx->current_state = STATE_WRITING;
            break;
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            break;
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 5;
            break;
        default:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 6;
            break;
    }
}

static void handle_writing(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_WRITE_COMPLETE:
            for (size_t i = 0; i < ctx->buffer_len && i < 10; i++) {
                putchar(ctx->buffer[i]);
            }
            putchar('\n');
            ctx->current_state = STATE_READING;
            break;
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            break;
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 7;
            break;
        default:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 8;
            break;
    }
}

static void handle_error(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            ctx->error_code = 0;
            break;
        default:
            break;
    }
}

static void handle_done(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            break;
        default:
            break;
    }
}

static void process_event(struct context *ctx, enum event ev) {
    switch (ctx->current_state) {
        case STATE_IDLE:
            handle_idle(ctx, ev);
            break;
        case STATE_READING:
            handle_reading(ctx, ev);
            break;
        case STATE_PROCESSING:
            handle_processing(ctx, ev);
            break;
        case STATE_WRITING:
            handle_writing(ctx, ev);
            break;
        case STATE_ERROR:
            handle_error(ctx, ev);
            break;
        case STATE_DONE:
            handle_done(ctx, ev);
            break;
    }
}

int main(void) {
    struct context ctx = {0};
    ctx.current_state = STATE_IDLE;
    
    enum event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE
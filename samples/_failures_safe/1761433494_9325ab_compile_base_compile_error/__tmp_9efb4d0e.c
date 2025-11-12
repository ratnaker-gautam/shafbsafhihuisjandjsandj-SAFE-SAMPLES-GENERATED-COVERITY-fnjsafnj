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
    int iteration_count;
};

static int validate_buffer(const char *buf, size_t len) {
    if (buf == NULL) return 0;
    if (len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (buf[i] == '\0') return 0;
    }
    return 1;
}

static void handle_idle(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_START:
            printf("Starting state machine\n");
            ctx->current_state = STATE_READING;
            ctx->iteration_count = 0;
            break;
        case EVENT_RESET:
            printf("Already in idle state\n");
            break;
        default:
            printf("Invalid event for idle state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

static void handle_reading(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_DATA_READY:
            if (ctx->iteration_count >= 5) {
                ctx->current_state = STATE_DONE;
                break;
            }
            snprintf(ctx->buffer, sizeof(ctx->buffer), "Data chunk %d", ctx->iteration_count + 1);
            ctx->buffer_len = strlen(ctx->buffer);
            if (validate_buffer(ctx->buffer, ctx->buffer_len)) {
                printf("Read: %s\n", ctx->buffer);
                ctx->current_state = STATE_PROCESSING;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for reading state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

static void handle_processing(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_PROCESS_COMPLETE:
            if (ctx->buffer_len > 0 && validate_buffer(ctx->buffer, ctx->buffer_len)) {
                for (size_t i = 0; i < ctx->buffer_len; i++) {
                    if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                        ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                    }
                }
                printf("Processed: %s\n", ctx->buffer);
                ctx->current_state = STATE_WRITING;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for processing state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

static void handle_writing(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_WRITE_COMPLETE:
            printf("Written: %s\n", ctx->buffer);
            ctx->iteration_count++;
            ctx->current_state = STATE_READING;
            break;
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for writing state\n");
            ctx->current_state = STATE_ERROR;
            break;
    }
}

static void handle_error(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            ctx->current_state = STATE_IDLE;
            ctx->error_code = 0;
            break;
        default:
            printf("Remaining in error state\n");
            break;
    }
}

static void handle_done(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Processing complete\n");
            break;
    }
}

static void process_event(struct context *ctx, enum event ev) {
    if (ctx == NULL) return;
    
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
        EVENT
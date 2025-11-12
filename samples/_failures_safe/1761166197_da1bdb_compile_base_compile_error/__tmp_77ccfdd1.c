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

static int validate_buffer_size(size_t len) {
    return len < sizeof(((struct context *)0)->buffer);
}

static void handle_idle(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_START:
            printf("Starting data processing\n");
            ctx->current_state = STATE_READING;
            ctx->buffer_len = 0;
            ctx->error_code = 0;
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
                strncpy(ctx->buffer, "Sample data for processing", 255);
            } else {
                snprintf(ctx->buffer, 255, "Data chunk %d", ctx->iteration_count + 1);
            }
            ctx->buffer[255] = '\0';
            ctx->buffer_len = strlen(ctx->buffer);
            if (validate_buffer_size(ctx->buffer_len)) {
                printf("Read data: %s\n", ctx->buffer);
                ctx->current_state = STATE_PROCESSING;
            } else {
                ctx->current_state = STATE_ERROR;
                ctx->error_code = 1;
            }
            break;
        case EVENT_RESET:
            printf("Resetting from reading state\n");
            ctx->current_state = STATE_IDLE;
            break;
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 2;
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
            if (ctx->buffer_len > 0 && validate_buffer_size(ctx->buffer_len)) {
                for (size_t i = 0; i < ctx->buffer_len; i++) {
                    if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                        ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                    }
                }
                printf("Processed data: %s\n", ctx->buffer);
                ctx->current_state = STATE_WRITING;
            } else {
                ctx->current_state = STATE_ERROR;
                ctx->error_code = 3;
            }
            break;
        case EVENT_RESET:
            printf("Resetting from processing state\n");
            ctx->current_state = STATE_IDLE;
            break;
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 4;
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
            printf("Data written successfully: %s\n", ctx->buffer);
            ctx->iteration_count++;
            if (ctx->iteration_count < 3) {
                ctx->current_state = STATE_READING;
            } else {
                ctx->current_state = STATE_DONE;
            }
            break;
        case EVENT_RESET:
            printf("Resetting from writing state\n");
            ctx->current_state = STATE_IDLE;
            break;
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            ctx->error_code = 5;
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
            printf("Resetting from error state (code: %d)\n", ctx->error_code);
            ctx->current_state = STATE_IDLE;
            ctx->error_code = 0;
            ctx->iteration_count = 0;
            break;
        default:
            printf("Can only reset from error state\n");
            break;
    }
}

static void handle_done(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            ctx->current_state = STATE_IDLE;
            ctx->iteration_count = 0;
            break;
        default:
            printf("Processing complete, can only reset\n");
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
        case STATE_PROCESS
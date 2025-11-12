//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    int processed_count;
    int max_items;
};

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

static enum state handle_idle(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_START:
            printf("Starting processing\n");
            ctx->processed_count = 0;
            ctx->buffer_len = 0;
            return STATE_READING;
        case EVENT_RESET:
            printf("Reset in idle state\n");
            return STATE_IDLE;
        default:
            printf("Invalid event for idle state\n");
            return STATE_ERROR;
    }
}

static enum state handle_reading(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_DATA_READY:
            if (ctx->processed_count >= ctx->max_items) {
                printf("Reached maximum items\n");
                return STATE_DONE;
            }
            printf("Data ready, moving to processing\n");
            return STATE_PROCESSING;
        case EVENT_RESET:
            printf("Reset in reading state\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error during reading\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for reading state\n");
            return STATE_ERROR;
    }
}

static enum state handle_processing(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_PROCESS_COMPLETE:
            if (ctx->buffer_len > 0) {
                for (size_t i = 0; i < ctx->buffer_len; i++) {
                    ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
                }
                ctx->processed_count++;
                printf("Processed item %d: %.*s\n", ctx->processed_count, (int)ctx->buffer_len, ctx->buffer);
            }
            return STATE_WRITING;
        case EVENT_RESET:
            printf("Reset in processing state\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error during processing\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for processing state\n");
            return STATE_ERROR;
    }
}

static enum state handle_writing(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_WRITE_COMPLETE:
            printf("Write completed\n");
            ctx->buffer_len = 0;
            return STATE_READING;
        case EVENT_RESET:
            printf("Reset in writing state\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("Error during writing\n");
            return STATE_ERROR;
        default:
            printf("Invalid event for writing state\n");
            return STATE_ERROR;
    }
}

static enum state handle_error(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            ctx->buffer_len = 0;
            ctx->processed_count = 0;
            return STATE_IDLE;
        default:
            printf("Only reset allowed in error state\n");
            return STATE_ERROR;
    }
}

static enum state handle_done(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            ctx->processed_count = 0;
            return STATE_IDLE;
        default:
            printf("Only reset allowed in done state\n");
            return STATE_DONE;
    }
}

static enum state state_machine_step(enum event ev, struct context *ctx) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ev, ctx);
        case STATE_READING:
            return handle_reading(ev, ctx);
        case STATE_PROCESSING:
            return handle_processing(ev, ctx);
        case STATE_WRITING:
            return handle_writing(ev, ctx);
        case STATE_ERROR:
            return handle_error(ev, ctx);
        case STATE_DONE:
            return handle_done(ev, ctx);
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_items = 5;
    
    char input[256];
    int event_input;
    
    printf("State Machine Demo - Enter events (0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR
//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    uint32_t processed_count;
};

static void handle_idle(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_START:
            printf("Starting data processing\n");
            ctx->current_state = STATE_READING;
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
            if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                ctx->buffer_len = strlen(ctx->buffer);
                if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
                    ctx->buffer[ctx->buffer_len - 1] = '\0';
                    ctx->buffer_len--;
                }
                if (ctx->buffer_len > 0) {
                    printf("Read %zu bytes: %s\n", ctx->buffer_len, ctx->buffer);
                    ctx->current_state = STATE_PROCESSING;
                } else {
                    printf("Empty input, staying in reading state\n");
                }
            } else {
                printf("No more input available\n");
                ctx->current_state = STATE_DONE;
            }
            break;
        case EVENT_RESET:
            printf("Resetting from reading state\n");
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
            for (size_t i = 0; i < ctx->buffer_len; i++) {
                if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                    ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                }
            }
            ctx->processed_count++;
            printf("Processed data: %s\n", ctx->buffer);
            ctx->current_state = STATE_WRITING;
            break;
        case EVENT_RESET:
            printf("Resetting from processing state\n");
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
            printf("Output: %s\n", ctx->buffer);
            ctx->current_state = STATE_READING;
            break;
        case EVENT_RESET:
            printf("Resetting from writing state\n");
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
            break;
        default:
            printf("Must reset from error state\n");
            break;
    }
}

static void handle_done(struct context *ctx, enum event ev) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            ctx->current_state = STATE_IDLE;
            break;
        default:
            printf("Processing complete, must reset\n");
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
    struct context ctx = {STATE_IDLE, {0}, 0, 0};
    char input[32];
    int running = 1;
    
    printf("State Machine Demo - Input Commands:\n");
    printf("start, read, process, write, reset, quit\n");
    
    while (running) {
        printf("Current state: ");
        switch (ctx.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
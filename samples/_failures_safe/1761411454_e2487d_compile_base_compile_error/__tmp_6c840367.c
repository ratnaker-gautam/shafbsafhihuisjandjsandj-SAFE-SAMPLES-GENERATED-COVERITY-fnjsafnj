//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
            printf("System reset\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static enum state handle_reading(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_DATA_READY:
            if (ctx->processed_count >= ctx->max_items) {
                return STATE_DONE;
            }
            printf("Enter data to process: ");
            if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
                return STATE_ERROR;
            }
            ctx->buffer_len = strlen(ctx->buffer);
            if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
                ctx->buffer[ctx->buffer_len - 1] = '\0';
                ctx->buffer_len--;
            }
            if (!validate_input(ctx->buffer, ctx->buffer_len)) {
                return STATE_ERROR;
            }
            return STATE_PROCESSING;
        case EVENT_RESET:
            printf("Reset during reading\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static enum state handle_processing(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_PROCESS_COMPLETE:
            for (size_t i = 0; i < ctx->buffer_len; i++) {
                ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
            }
            printf("Processed: %s\n", ctx->buffer);
            ctx->processed_count++;
            return STATE_WRITING;
        case EVENT_RESET:
            printf("Reset during processing\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static enum state handle_writing(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_WRITE_COMPLETE:
            printf("Data written successfully\n");
            if (ctx->processed_count < ctx->max_items) {
                return STATE_READING;
            } else {
                return STATE_DONE;
            }
        case EVENT_RESET:
            printf("Reset during writing\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static enum state handle_error(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static enum state handle_done(enum event ev, struct context *ctx) {
    switch (ev) {
        case EVENT_RESET:
            printf("Resetting from done state\n");
            return STATE_IDLE;
        default:
            return STATE_DONE;
    }
}

static enum state state_machine_step(enum event ev, struct context *ctx) {
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
    struct context ctx = {STATE_IDLE, {0}, 0, 0, 0};
    char input[32];
    enum event current_event;
    
    printf("Enter maximum number of items to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    if (sscanf(input, "%d", &ctx.max_items) != 1 || ctx.max_items <= 0 || ctx.max_items > 100) {
        printf("Invalid input. Using default of 3 items.\n");
        ctx.max_items = 3;
    }
    
    current_event = EVENT_START;
    
    while (1) {
        ctx.current_state = state_machine_step(current_event, &ctx);
        
        if (ctx.current_state == STATE_D
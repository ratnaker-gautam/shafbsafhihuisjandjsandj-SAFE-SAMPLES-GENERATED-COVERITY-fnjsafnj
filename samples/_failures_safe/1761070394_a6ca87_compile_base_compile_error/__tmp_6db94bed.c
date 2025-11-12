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

static void process_data(struct context *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (islower((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
        }
    }
}

static enum state handle_event(struct context *ctx, enum event ev) {
    if (ctx == NULL) {
        return STATE_ERROR;
    }

    switch (ctx->current_state) {
        case STATE_IDLE:
            switch (ev) {
                case EVENT_START:
                    ctx->processed_count = 0;
                    ctx->buffer_len = 0;
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            break;

        case STATE_READING:
            switch (ev) {
                case EVENT_DATA_READY:
                    if (ctx->buffer_len > 0 && validate_input(ctx->buffer, ctx->buffer_len)) {
                        return STATE_PROCESSING;
                    } else {
                        return STATE_ERROR;
                    }
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
            break;

        case STATE_PROCESSING:
            switch (ev) {
                case EVENT_PROCESS_COMPLETE:
                    process_data(ctx);
                    if (ctx->processed_count < ctx->max_items) {
                        return STATE_WRITING;
                    } else {
                        return STATE_DONE;
                    }
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
            break;

        case STATE_WRITING:
            switch (ev) {
                case EVENT_WRITE_COMPLETE:
                    ctx->processed_count++;
                    ctx->buffer_len = 0;
                    return STATE_READING;
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
            break;

        case STATE_ERROR:
            switch (ev) {
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                default:
                    return STATE_ERROR;
            }
            break;

        case STATE_DONE:
            switch (ev) {
                case EVENT_RESET:
                    ctx->buffer_len = 0;
                    ctx->processed_count = 0;
                    return STATE_IDLE;
                default:
                    return STATE_DONE;
            }
            break;

        default:
            return STATE_ERROR;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_items = 3;

    printf("State Machine Demo - Enter up to %d text items (alphanumeric only)\n", ctx.max_items);
    printf("Commands: 'start' to begin, 'reset' to reset, 'quit' to exit\n");

    char input[256];
    while (1) {
        printf("Current state: ");
        switch (ctx.current_state) {
            case STATE_IDLE: printf("IDLE\n"); break;
            case STATE_READING: printf("READING\n"); break;
            case STATE_PROCESSING: printf("PROCESSING\n"); break;
            case STATE_WRITING: printf("WRITING\n"); break;
            case STATE_ERROR: printf("ERROR\n"); break;
            case STATE_DONE: printf("DONE\n"); break;
        }

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }

        if (strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "start") == 0) {
            ctx.current_state = handle_event(&ctx, EVENT_START);
        } else if (strcmp(input, "reset") == 0) {
            ctx.current_state = handle_event(&ctx, EVENT_RESET);
        } else {
            if (
//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void context_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

int is_valid_char(char c) {
    return isprint(c) || c == ' ' || c == '\t' || c == '\n';
}

int handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    if (is_valid_char(input)) {
        if (input != ' ' && input != '\t' && input != '\n') {
            ctx->current_state = STATE_READING;
            if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
                ctx->buffer[ctx->buffer_pos++] = input;
                ctx->char_count++;
            }
        }
        return 0;
    }
    ctx->current_state = STATE_ERROR;
    return -1;
}

int handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    if (!is_valid_char(input)) {
        ctx->current_state = STATE_ERROR;
        return -1;
    }
    if (input == ' ' || input == '\t' || input == '\n') {
        ctx->current_state = STATE_PROCESSING;
        return 0;
    }
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
    } else {
        ctx->current_state = STATE_ERROR;
        return -1;
    }
    return 0;
}

int handle_processing(struct context *ctx) {
    if (ctx == NULL) return -1;
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
    }
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->current_state = STATE_IDLE;
    return 0;
}

int handle_done(struct context *ctx) {
    if (ctx == NULL) return -1;
    printf("Processing complete. Words: %d, Characters: %d\n", 
           ctx->word_count, ctx->char_count);
    return 0;
}

int handle_error(struct context *ctx) {
    if (ctx == NULL) return -1;
    printf("Error: Invalid input encountered\n");
    return -1;
}

int process_input(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            handle_processing(ctx);
            return handle_idle(ctx, input);
        case STATE_DONE:
            return 0;
        case STATE_ERROR:
            return -1;
        default:
            ctx->current_state = STATE_ERROR;
            return -1;
    }
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    printf("Enter text (Ctrl+D to end): ");
    int c;
    while ((c = getchar()) != EOF) {
        if (process_input(&ctx, (char)c) != 0) {
            break;
        }
    }
    if (ctx.current_state == STATE_READING) {
        handle_processing(&ctx);
    }
    if (ctx.current_state != STATE_ERROR) {
        ctx.current_state = STATE_DONE;
        handle_done(&ctx);
    } else {
        handle_error(&ctx);
    }
    return 0;
}
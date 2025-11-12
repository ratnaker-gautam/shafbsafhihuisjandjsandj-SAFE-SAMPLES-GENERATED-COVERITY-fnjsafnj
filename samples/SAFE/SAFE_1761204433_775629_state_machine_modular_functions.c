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

int is_valid_char(int c) {
    return c >= 0 && c <= 127;
}

int is_word_separator(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int handle_idle(struct context *ctx, int input) {
    if (!is_valid_char(input)) {
        return STATE_ERROR;
    }
    if (input == EOF) {
        return STATE_DONE;
    }
    if (!is_word_separator(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)input;
            ctx->char_count++;
        }
        return STATE_READING;
    }
    return STATE_IDLE;
}

int handle_reading(struct context *ctx, int input) {
    if (!is_valid_char(input)) {
        return STATE_ERROR;
    }
    if (input == EOF) {
        ctx->word_count++;
        return STATE_DONE;
    }
    if (is_word_separator(input)) {
        ctx->word_count++;
        ctx->buffer_pos = 0;
        return STATE_IDLE;
    }
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = (char)input;
        ctx->char_count++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

int handle_processing(struct context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->word_count++;
    }
    return STATE_DONE;
}

int handle_error(struct context *ctx) {
    return STATE_ERROR;
}

void process_state_machine(struct context *ctx) {
    int input;
    
    while (ctx->current_state != STATE_DONE && ctx->current_state != STATE_ERROR) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                input = getchar();
                ctx->current_state = handle_idle(ctx, input);
                break;
            case STATE_READING:
                input = getchar();
                ctx->current_state = handle_reading(ctx, input);
                break;
            case STATE_PROCESSING:
                ctx->current_state = handle_processing(ctx);
                break;
            case STATE_ERROR:
                ctx->current_state = handle_error(ctx);
                break;
            default:
                ctx->current_state = STATE_ERROR;
                break;
        }
    }
}

void print_results(const struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->current_state == STATE_ERROR) {
        printf("Error: Invalid input encountered\n");
        return;
    }
    
    printf("Words: %d\n", ctx->word_count);
    printf("Characters: %d\n", ctx->char_count);
}

int main(void) {
    struct context ctx;
    
    context_init(&ctx);
    process_state_machine(&ctx);
    print_results(&ctx);
    
    if (ctx.current_state == STATE_ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
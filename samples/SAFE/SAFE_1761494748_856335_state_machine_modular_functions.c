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
    return isprint(c) || c == '\n' || c == '\t' || c == ' ';
}

int handle_idle(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    if (input == '\n') return STATE_IDLE;
    if (is_valid_char(input)) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
        return STATE_READING;
    }
    return STATE_ERROR;
}

int handle_reading(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    if (input == '\n') {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        return STATE_PROCESSING;
    }
    if (is_valid_char(input)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
            return STATE_READING;
        } else {
            return STATE_ERROR;
        }
    }
    return STATE_ERROR;
}

int handle_processing(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    if (input == '\n') {
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_pos = 0;
        return STATE_DONE;
    }
    return STATE_ERROR;
}

int handle_done(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    if (input == '\n') {
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int handle_error(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    if (input == '\n') {
        context_init(ctx);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

int process_input(struct context *ctx, char input) {
    if (ctx == NULL) return -1;
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle(ctx, input);
        case STATE_READING:
            return handle_reading(ctx, input);
        case STATE_PROCESSING:
            return handle_processing(ctx, input);
        case STATE_DONE:
            return handle_done(ctx, input);
        case STATE_ERROR:
            return handle_error(ctx, input);
        default:
            return STATE_ERROR;
    }
}

void print_stats(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Words: %d, Characters: %d\n", ctx->word_count, ctx->char_count);
}

int main(void) {
    struct context ctx;
    context_init(&ctx);
    
    printf("Enter text lines (empty line to finish):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\r') continue;
        
        enum state new_state = process_input(&ctx, (char)c);
        if (new_state == -1) {
            fprintf(stderr, "Error: Invalid context\n");
            return 1;
        }
        ctx.current_state = new_state;
        
        if (ctx.current_state == STATE_DONE) {
            print_stats(&ctx);
            context_init(&ctx);
        } else if (ctx.current_state == STATE_ERROR) {
            fprintf(stderr, "Error: Invalid input detected\n");
            context_init(&ctx);
        }
        
        if (c == '\n' && ctx.current_state == STATE_IDLE) {
            break;
        }
    }
    
    if (ctx.current_state == STATE_READING || ctx.current_state == STATE_PROCESSING) {
        print_stats(&ctx);
    }
    
    return 0;
}
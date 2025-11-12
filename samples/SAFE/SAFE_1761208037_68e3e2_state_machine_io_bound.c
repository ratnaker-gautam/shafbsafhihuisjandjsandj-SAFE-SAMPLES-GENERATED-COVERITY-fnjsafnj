//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
    int line_count;
};

void init_context(struct Context *ctx) {
    ctx->current_state = INIT;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    ctx->line_count = 0;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

enum State handle_init(struct Context *ctx, char input) {
    if (input == EOF) {
        return DONE;
    }
    if (input == '\n') {
        ctx->line_count++;
    }
    ctx->char_count++;
    if (!is_whitespace(input)) {
        ctx->buffer[ctx->buffer_pos++] = input;
        return READING;
    }
    return INIT;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (input == EOF) {
        ctx->word_count++;
        return OUTPUT;
    }
    ctx->char_count++;
    if (input == '\n') {
        ctx->line_count++;
    }
    if (is_whitespace(input)) {
        ctx->word_count++;
        return PROCESSING;
    }
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = input;
    } else {
        return ERROR;
    }
    return READING;
}

enum State handle_processing(struct Context *ctx, char input) {
    if (input == EOF) {
        return OUTPUT;
    }
    ctx->char_count++;
    if (input == '\n') {
        ctx->line_count++;
    }
    if (!is_whitespace(input)) {
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        ctx->buffer_pos = 0;
        ctx->buffer[ctx->buffer_pos++] = input;
        return READING;
    }
    return PROCESSING;
}

enum State handle_output(struct Context *ctx) {
    printf("Words: %d\n", ctx->word_count);
    printf("Characters: %d\n", ctx->char_count);
    printf("Lines: %d\n", ctx->line_count);
    return DONE;
}

enum State handle_error(struct Context *ctx) {
    printf("Error: Buffer overflow\n");
    return DONE;
}

enum State transition(struct Context *ctx, char input) {
    switch (ctx->current_state) {
        case INIT:
            return handle_init(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case OUTPUT:
            return handle_output(ctx);
        case ERROR:
            return handle_error(ctx);
        case DONE:
            return DONE;
        default:
            return ERROR;
    }
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);
    
    int c;
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        c = getchar();
        if (c == EOF && ctx.current_state == INIT) {
            break;
        }
        ctx.current_state = transition(&ctx, (char)c);
    }
    
    if (ctx.current_state == OUTPUT) {
        ctx.current_state = transition(&ctx, EOF);
    }
    
    return 0;
}
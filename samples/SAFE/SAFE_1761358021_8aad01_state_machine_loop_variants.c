//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
}

enum State handle_idle(struct Context *ctx, int c) {
    if (c == EOF) return DONE;
    if (isspace(c)) return IDLE;
    if (isprint(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        }
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int c) {
    if (c == EOF) {
        ctx->word_count++;
        return WRITING;
    }
    if (isspace(c)) {
        ctx->word_count++;
        return PROCESSING;
    }
    if (isprint(c)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)c;
            ctx->char_count++;
        }
        return READING;
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        printf("Word: %s\n", ctx->buffer);
        ctx->buffer_pos = 0;
    }
    return WRITING;
}

enum State handle_writing(struct Context *ctx) {
    printf("Stats: %d words, %d characters\n", ctx->word_count, ctx->char_count);
    return IDLE;
}

enum State handle_error(struct Context *ctx) {
    printf("Error: Invalid input character encountered\n");
    initialize_context(ctx);
    return IDLE;
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    int input_char;
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                input_char = getchar();
                ctx.current_state = handle_idle(&ctx, input_char);
                break;
            case READING:
                input_char = getchar();
                ctx.current_state = handle_reading(&ctx, input_char);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case WRITING:
                ctx.current_state = handle_writing(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case DONE:
                break;
        }
    }
    
    if (ctx.current_state == DONE) {
        if (ctx.buffer_pos > 0) {
            ctx.buffer[ctx.buffer_pos] = '\0';
            printf("Final word: %s\n", ctx.buffer);
            ctx.word_count++;
        }
        printf("Final stats: %d words, %d characters\n", ctx.word_count, ctx.char_count);
    }
    
    return ctx.current_state == DONE ? EXIT_SUCCESS : EXIT_FAILURE;
}
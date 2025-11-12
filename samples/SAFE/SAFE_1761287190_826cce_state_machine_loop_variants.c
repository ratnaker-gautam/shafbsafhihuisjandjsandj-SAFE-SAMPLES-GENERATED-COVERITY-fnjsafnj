//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
    int line_count;
};

void state_idle(struct Context *ctx, int ch) {
    if (ch == EOF) {
        ctx->current_state = DONE;
    } else if (ch == '\n') {
        ctx->line_count++;
    } else if (!isspace(ch)) {
        ctx->buffer[ctx->buffer_pos++] = (char)ch;
        ctx->char_count++;
        ctx->current_state = READING;
    }
}

void state_reading(struct Context *ctx, int ch) {
    if (ch == EOF || isspace(ch)) {
        if (ctx->buffer_pos > 0) {
            ctx->buffer[ctx->buffer_pos] = '\0';
            ctx->word_count++;
            ctx->buffer_pos = 0;
        }
        if (ch == '\n') {
            ctx->line_count++;
        }
        ctx->current_state = (ch == EOF) ? DONE : IDLE;
    } else {
        if (ctx->buffer_pos < 255) {
            ctx->buffer[ctx->buffer_pos++] = (char)ch;
            ctx->char_count++;
        } else {
            ctx->current_state = ERROR;
        }
    }
}

void state_processing(struct Context *ctx, int ch) {
    if (ctx->buffer_pos > 0) {
        for (size_t i = 0; i < ctx->buffer_pos; i++) {
            ctx->buffer[i] = (char)toupper(ctx->buffer[i]);
        }
    }
    ctx->current_state = WRITING;
}

void state_writing(struct Context *ctx, int ch) {
    if (ctx->buffer_pos > 0) {
        printf("%s", ctx->buffer);
        if (ch != EOF) {
            printf(" ");
        }
        ctx->buffer_pos = 0;
    }
    ctx->current_state = (ch == EOF) ? DONE : IDLE;
}

void state_error(struct Context *ctx, int ch) {
    fprintf(stderr, "Error: Buffer overflow\n");
    ctx->current_state = DONE;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.char_count = 0;
    ctx.line_count = 0;

    int ch;
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        ch = getchar();
        
        switch (ctx.current_state) {
            case IDLE:
                state_idle(&ctx, ch);
                break;
            case READING:
                state_reading(&ctx, ch);
                if (ctx.current_state == IDLE && ctx.buffer_pos == 0) {
                    ctx.current_state = PROCESSING;
                }
                break;
            case PROCESSING:
                state_processing(&ctx, ch);
                break;
            case WRITING:
                state_writing(&ctx, ch);
                break;
            case ERROR:
                state_error(&ctx, ch);
                break;
            case DONE:
                break;
        }
    }

    if (ctx.current_state == DONE) {
        printf("\nWords: %d, Characters: %d, Lines: %d\n", 
               ctx.word_count, ctx.char_count, ctx.line_count);
    }

    return (ctx.current_state == ERROR) ? EXIT_FAILURE : EXIT_SUCCESS;
}
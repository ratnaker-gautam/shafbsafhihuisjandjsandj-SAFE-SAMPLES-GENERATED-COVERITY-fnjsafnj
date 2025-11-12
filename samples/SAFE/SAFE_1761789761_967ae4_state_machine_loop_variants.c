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

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    ctx->line_count = 0;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int handle_idle_state(struct Context *ctx, char input) {
    if (input == EOF) {
        ctx->current_state = DONE;
        return 1;
    }
    if (!is_whitespace(input)) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
        ctx->current_state = READING;
    } else if (input == '\n') {
        ctx->line_count++;
    }
    return 0;
}

int handle_reading_state(struct Context *ctx, char input) {
    if (input == EOF) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
        }
        ctx->current_state = DONE;
        return 1;
    }
    if (is_whitespace(input)) {
        if (ctx->buffer_pos > 0) {
            ctx->word_count++;
            ctx->buffer_pos = 0;
        }
        if (input == '\n') {
            ctx->line_count++;
        }
        ctx->current_state = IDLE;
    } else {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        } else {
            ctx->current_state = ERROR;
        }
    }
    return 0;
}

int handle_processing_state(struct Context *ctx) {
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        ctx->buffer[i] = toupper((unsigned char)ctx->buffer[i]);
    }
    ctx->current_state = WRITING;
    return 0;
}

int handle_writing_state(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        for (size_t i = 0; i < ctx->buffer_pos; i++) {
            putchar(ctx->buffer[i]);
        }
        putchar(' ');
        ctx->buffer_pos = 0;
    }
    ctx->current_state = IDLE;
    return 0;
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    int input_char;
    int should_exit = 0;

    while (!should_exit) {
        input_char = getchar();
        switch (ctx.current_state) {
            case IDLE:
                should_exit = handle_idle_state(&ctx, input_char);
                break;
            case READING:
                should_exit = handle_reading_state(&ctx, input_char);
                if (ctx.current_state == IDLE && ctx.buffer_pos > 0) {
                    ctx.current_state = PROCESSING;
                }
                break;
            case PROCESSING:
                should_exit = handle_processing_state(&ctx);
                break;
            case WRITING:
                should_exit = handle_writing_state(&ctx);
                break;
            case ERROR:
                fprintf(stderr, "Error: Buffer overflow\n");
                return 1;
            case DONE:
                should_exit = 1;
                break;
        }
    }

    printf("\nStatistics: %d lines, %d words, %d characters\n",
           ctx.line_count, ctx.word_count, ctx.char_count);
    return 0;
}
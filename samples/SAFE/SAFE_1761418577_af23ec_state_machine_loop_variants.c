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

void state_idle(struct Context *ctx, char input) {
    if (input == '\n') {
        ctx->line_count++;
    } else if (!isspace(input)) {
        ctx->current_state = READING;
        ctx->buffer[0] = input;
        ctx->buffer_pos = 1;
        ctx->char_count++;
    }
}

void state_reading(struct Context *ctx, char input) {
    if (isspace(input)) {
        if (input == '\n') {
            ctx->line_count++;
        }
        ctx->current_state = PROCESSING;
    } else {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        } else {
            ctx->current_state = ERROR;
        }
    }
}

void state_processing(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->word_count++;
    }
    ctx->buffer_pos = 0;
    ctx->current_state = WRITING;
}

void state_writing(struct Context *ctx) {
    if (ctx->buffer[0] != '\0') {
        printf("Word %d: %s\n", ctx->word_count, ctx->buffer);
        ctx->buffer[0] = '\0';
    }
    ctx->current_state = IDLE;
}

void state_error(struct Context *ctx) {
    fprintf(stderr, "Error: Buffer overflow\n");
    ctx->current_state = DONE;
}

void process_input(struct Context *ctx, char input) {
    switch (ctx->current_state) {
        case IDLE:
            state_idle(ctx, input);
            break;
        case READING:
            state_reading(ctx, input);
            break;
        case PROCESSING:
            state_processing(ctx);
            break;
        case WRITING:
            state_writing(ctx);
            break;
        case ERROR:
            state_error(ctx);
            break;
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.char_count = 0;
    ctx.line_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    printf("Enter text (Ctrl+D to end):\n");

    int input_char;
    while (ctx.current_state != DONE && (input_char = getchar()) != EOF) {
        if (input_char == '\0') {
            continue;
        }
        process_input(&ctx, (char)input_char);
    }

    if (ctx.current_state == READING) {
        state_processing(&ctx);
        if (ctx.current_state == WRITING) {
            state_writing(&ctx);
        }
    }

    if (ctx.current_state != ERROR) {
        printf("\nStatistics:\n");
        printf("Words: %d\n", ctx.word_count);
        printf("Characters: %d\n", ctx.char_count);
        printf("Lines: %d\n", ctx.line_count);
    }

    return ctx.current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
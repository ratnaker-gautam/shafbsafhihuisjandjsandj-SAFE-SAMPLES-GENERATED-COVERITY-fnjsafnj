//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING_SPACE,
    FINISHED,
    ERROR
};

struct context {
    enum state current;
    int number_count;
    int word_count;
    char buffer[64];
    size_t buf_pos;
};

void init_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(int c) {
    return c == EOF || c == ' ' || c == '\t' || c == '\n' || isalnum(c);
}

void process_char(struct context *ctx, int c) {
    if (ctx == NULL || !is_valid_char(c)) {
        ctx->current = ERROR;
        return;
    }

    switch (ctx->current) {
        case START:
            if (c == EOF) {
                ctx->current = FINISHED;
            } else if (isdigit(c)) {
                ctx->buffer[ctx->buf_pos++] = (char)c;
                ctx->current = READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->buffer[ctx->buf_pos++] = (char)c;
                ctx->current = READING_WORD;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->current = SKIPPING_SPACE;
            } else {
                ctx->current = ERROR;
            }
            break;

        case READING_NUMBER:
            if (c == EOF || c == ' ' || c == '\t' || c == '\n') {
                if (ctx->buf_pos > 0) {
                    ctx->number_count++;
                }
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = (c == EOF) ? FINISHED : SKIPPING_SPACE;
            } else if (isdigit(c)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = (char)c;
                } else {
                    ctx->current = ERROR;
                }
            } else {
                ctx->current = ERROR;
            }
            break;

        case READING_WORD:
            if (c == EOF || c == ' ' || c == '\t' || c == '\n') {
                if (ctx->buf_pos > 0) {
                    ctx->word_count++;
                }
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = (c == EOF) ? FINISHED : SKIPPING_SPACE;
            } else if (isalpha(c)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = (char)c;
                } else {
                    ctx->current = ERROR;
                }
            } else {
                ctx->current = ERROR;
            }
            break;

        case SKIPPING_SPACE:
            if (c == EOF) {
                ctx->current = FINISHED;
            } else if (isdigit(c)) {
                ctx->buffer[ctx->buf_pos++] = (char)c;
                ctx->current = READING_NUMBER;
            } else if (isalpha(c)) {
                ctx->buffer[ctx->buf_pos++] = (char)c;
                ctx->current = READING_WORD;
            } else if (c != ' ' && c != '\t' && c != '\n') {
                ctx->current = ERROR;
            }
            break;

        case FINISHED:
        case ERROR:
            break;
    }
}

int main(void) {
    struct context ctx;
    init_context(&ctx);

    printf("Enter text (Ctrl+D to end):\n");

    int c;
    while ((c = getchar()) != EOF && ctx.current != FINISHED && ctx.current != ERROR) {
        process_char(&ctx, c);
    }

    if (ctx.current != FINISHED) {
        process_char(&ctx, EOF);
    }

    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
        return EXIT_FAILURE;
    }

    printf("Numbers: %d\nWords: %d\n", ctx.number_count, ctx.word_count);
    return EXIT_SUCCESS;
}
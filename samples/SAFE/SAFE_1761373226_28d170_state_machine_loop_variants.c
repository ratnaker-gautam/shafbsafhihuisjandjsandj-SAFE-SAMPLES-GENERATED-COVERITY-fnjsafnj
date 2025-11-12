//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
    char buffer[256];
    size_t buf_pos;
};

void state_machine_reset(struct context *ctx) {
    ctx->current = START;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_input_char(int c) {
    return (c >= 0 && c <= 127) && (isalnum(c) || isspace(c) || c == EOF);
}

void process_char(struct context *ctx, int c) {
    if (!is_valid_input_char(c)) {
        ctx->current = ERROR;
        return;
    }

    switch (ctx->current) {
        case START:
            if (isdigit(c)) {
                ctx->current = READING_NUMBER;
                ctx->buffer[ctx->buf_pos++] = (char)c;
            } else if (isalpha(c)) {
                ctx->current = READING_WORD;
                ctx->buffer[ctx->buf_pos++] = (char)c;
            } else if (isspace(c)) {
                ctx->current = SKIPPING_SPACE;
            } else if (c == EOF) {
                ctx->current = FINISHED;
            }
            break;

        case READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = (char)c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (isspace(c) || c == EOF) {
                ctx->number_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = (c == EOF) ? FINISHED : SKIPPING_SPACE;
            } else {
                ctx->current = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = (char)c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (isspace(c) || c == EOF) {
                ctx->word_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = (c == EOF) ? FINISHED : SKIPPING_SPACE;
            } else {
                ctx->current = ERROR;
            }
            break;

        case SKIPPING_SPACE:
            if (isdigit(c)) {
                ctx->current = READING_NUMBER;
                ctx->buffer[ctx->buf_pos++] = (char)c;
            } else if (isalpha(c)) {
                ctx->current = READING_WORD;
                ctx->buffer[ctx->buf_pos++] = (char)c;
            } else if (c == EOF) {
                ctx->current = FINISHED;
            } else if (!isspace(c)) {
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
    state_machine_reset(&ctx);

    printf("Enter text (numbers and words): ");
    fflush(stdout);

    int c;
    while (ctx.current != FINISHED && ctx.current != ERROR) {
        c = getchar();
        if (c == EOF) {
            process_char(&ctx, EOF);
            break;
        }
        process_char(&ctx, c);
    }

    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);

    return 0;
}
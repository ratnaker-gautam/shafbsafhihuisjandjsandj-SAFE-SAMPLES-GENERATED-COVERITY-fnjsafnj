//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State { START, READING_NUM, READING_WORD, ERROR, DONE };

struct Context {
    enum State current;
    int num_count;
    int word_count;
    char buffer[64];
    size_t buf_pos;
};

void state_machine_reset(struct Context *ctx) {
    ctx->current = START;
    ctx->num_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(int c) {
    return c == EOF || c == '\n' || c == ' ' || c == '\t' || 
           (c >= '0' && c <= '9') || 
           (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z');
}

void process_char(struct Context *ctx, int c) {
    if (!is_valid_char(c)) {
        ctx->current = ERROR;
        return;
    }

    switch (ctx->current) {
        case START:
            if (c == EOF || c == '\n') {
                ctx->current = DONE;
            } else if (c == ' ' || c == '\t') {
                break;
            } else if (c >= '0' && c <= '9') {
                ctx->current = READING_NUM;
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = (char)c;
                }
            } else if (isalpha(c)) {
                ctx->current = READING_WORD;
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = (char)c;
                }
            }
            break;

        case READING_NUM:
            if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                ctx->num_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = (c == '\n' || c == EOF) ? DONE : START;
            } else if (c >= '0' && c <= '9') {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = (char)c;
                }
            } else {
                ctx->current = ERROR;
            }
            break;

        case READING_WORD:
            if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                ctx->word_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = (c == '\n' || c == EOF) ? DONE : START;
            } else if (isalpha(c)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = (char)c;
                }
            } else {
                ctx->current = ERROR;
            }
            break;

        case ERROR:
        case DONE:
            break;
    }
}

int main(void) {
    struct Context ctx;
    state_machine_reset(&ctx);

    printf("Enter text (numbers and words): ");

    int ch;
    while ((ch = getchar()) != EOF) {
        process_char(&ctx, ch);
        if (ctx.current == ERROR || ctx.current == DONE) {
            break;
        }
    }

    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }

    if (ctx.current == READING_NUM || ctx.current == READING_WORD) {
        if (ctx.current == READING_NUM) {
            ctx.num_count++;
        } else {
            ctx.word_count++;
        }
    }

    printf("Numbers: %d\nWords: %d\n", ctx.num_count, ctx.word_count);
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state { START, READING_NUM, READING_WORD, ERROR, DONE };

struct context {
    enum state current;
    int num_count;
    int word_count;
    char buffer[256];
    int buf_pos;
};

void state_machine_reset(struct context *ctx) {
    ctx->current = START;
    ctx->num_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

void process_char(struct context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current = ERROR;
        return;
    }

    switch (ctx->current) {
        case START:
            if (isdigit(c)) {
                ctx->current = READING_NUM;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (isalpha(c)) {
                ctx->current = READING_WORD;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->current = START;
            }
            break;

        case READING_NUM:
            if (isdigit(c)) {
                if (ctx->buf_pos < 255) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->num_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = START;
            } else {
                ctx->current = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->buf_pos < 255) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->word_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = START;
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
    struct context ctx;
    state_machine_reset(&ctx);

    printf("Enter text (alphanumeric only, max 4096 chars): ");
    fflush(stdout);

    char input[4097];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }

    if (len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    int i = 0;
    while (i < len && ctx.current != ERROR) {
        process_char(&ctx, input[i]);
        i++;
    }

    if (ctx.current == READING_NUM) {
        ctx.num_count++;
    } else if (ctx.current == READING_WORD) {
        ctx.word_count++;
    }

    if (ctx.current == ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Numbers: %d\nWords: %d\n", ctx.num_count, ctx.word_count);
    return 0;
}
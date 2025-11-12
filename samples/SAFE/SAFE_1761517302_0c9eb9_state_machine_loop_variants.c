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
    int buf_pos;
};

void state_machine_reset(struct Context *ctx) {
    ctx->current = START;
    ctx->num_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\t';
}

void process_char(struct Context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current = ERROR;
        return;
    }

    switch (ctx->current) {
        case START:
            if (isdigit((unsigned char)c)) {
                ctx->current = READING_NUM;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (isalpha((unsigned char)c)) {
                ctx->current = READING_WORD;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                ctx->current = START;
            }
            break;

        case READING_NUM:
            if (isdigit((unsigned char)c)) {
                if (ctx->buf_pos < 63) {
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
            if (isalpha((unsigned char)c)) {
                if (ctx->buf_pos < 63) {
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
    struct Context ctx;
    state_machine_reset(&ctx);

    printf("Enter text (numbers and words separated by spaces): ");
    fflush(stdout);

    char input[1024];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
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

    printf("Numbers: %d\n", ctx.num_count);
    printf("Words: %d\n", ctx.word_count);

    return 0;
}
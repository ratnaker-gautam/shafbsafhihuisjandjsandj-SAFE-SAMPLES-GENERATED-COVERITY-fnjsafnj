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
    char buffer[256];
    int buf_pos;
};

void state_machine_init(struct Context *ctx) {
    ctx->current = START;
    ctx->num_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum State handle_start(struct Context *ctx, char c) {
    if (c == '\n' || c == '\0') {
        return DONE;
    } else if (isdigit(c)) {
        ctx->buffer[ctx->buf_pos++] = c;
        return READING_NUM;
    } else if (isalpha(c)) {
        ctx->buffer[ctx->buf_pos++] = c;
        return READING_WORD;
    } else if (isspace(c)) {
        return START;
    } else {
        return ERROR;
    }
}

enum State handle_reading_num(struct Context *ctx, char c) {
    if (isdigit(c)) {
        if (ctx->buf_pos < 255) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_NUM;
    } else if (isspace(c) || c == '\0') {
        ctx->num_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\n' || c == '\0') {
            return DONE;
        }
        return START;
    } else {
        return ERROR;
    }
}

enum State handle_reading_word(struct Context *ctx, char c) {
    if (isalpha(c)) {
        if (ctx->buf_pos < 255) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_WORD;
    } else if (isspace(c) || c == '\0') {
        ctx->word_count++;
        ctx->buf_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (c == '\n' || c == '\0') {
            return DONE;
        }
        return START;
    } else {
        return ERROR;
    }
}

enum State process_char(struct Context *ctx, char c) {
    switch (ctx->current) {
        case START:
            return handle_start(ctx, c);
        case READING_NUM:
            return handle_reading_num(ctx, c);
        case READING_WORD:
            return handle_reading_word(ctx, c);
        case ERROR:
            return ERROR;
        case DONE:
            return DONE;
        default:
            return ERROR;
    }
}

int main(void) {
    struct Context ctx;
    state_machine_init(&ctx);
    
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
    
    size_t i = 0;
    while (i < len && ctx.current != DONE && ctx.current != ERROR) {
        ctx.current = process_char(&ctx, input[i]);
        i++;
    }
    
    if (ctx.current == ERROR) {
        fprintf(stderr, "Invalid input format\n");
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
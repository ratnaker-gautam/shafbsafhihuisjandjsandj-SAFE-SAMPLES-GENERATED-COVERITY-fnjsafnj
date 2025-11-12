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

void reset_context(struct Context *ctx) {
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
    } else if (isspace(c)) {
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
    } else if (isspace(c)) {
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
    reset_context(&ctx);
    
    char input[1024];
    printf("Enter text (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    int i = 0;
    while (input[i] != '\0') {
        ctx.current = process_char(&ctx, input[i]);
        if (ctx.current == ERROR) {
            printf("Invalid input format\n");
            return 1;
        }
        i++;
    }
    
    if (ctx.current == READING_NUM) {
        ctx.num_count++;
    } else if (ctx.current == READING_WORD) {
        ctx.word_count++;
    }
    
    printf("Numbers: %d\nWords: %d\n", ctx.num_count, ctx.word_count);
    
    return 0;
}
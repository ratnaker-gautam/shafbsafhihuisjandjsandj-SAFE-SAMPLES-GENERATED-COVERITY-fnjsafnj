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

void reset_buffer(struct Context *ctx) {
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buf_pos = 0;
}

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

enum State handle_start(struct Context *ctx, char c) {
    if (isdigit(c)) {
        reset_buffer(ctx);
        ctx->buffer[ctx->buf_pos++] = c;
        return READING_NUM;
    } else if (isalpha(c)) {
        reset_buffer(ctx);
        ctx->buffer[ctx->buf_pos++] = c;
        return READING_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        return START;
    }
    return ERROR;
}

enum State handle_reading_num(struct Context *ctx, char c) {
    if (isdigit(c)) {
        if (ctx->buf_pos < 63) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_NUM;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->num_count++;
        return START;
    } else if (isalpha(c)) {
        return ERROR;
    }
    return ERROR;
}

enum State handle_reading_word(struct Context *ctx, char c) {
    if (isalpha(c)) {
        if (ctx->buf_pos < 63) {
            ctx->buffer[ctx->buf_pos++] = c;
        }
        return READING_WORD;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        ctx->word_count++;
        return START;
    } else if (isdigit(c)) {
        return ERROR;
    }
    return ERROR;
}

enum State process_char(struct Context *ctx, char c) {
    if (!is_valid_char(c)) {
        return ERROR;
    }
    
    switch (ctx->current) {
        case START:
            return handle_start(ctx, c);
        case READING_NUM:
            return handle_reading_num(ctx, c);
        case READING_WORD:
            return handle_reading_word(ctx, c);
        case ERROR:
        case DONE:
            return ctx->current;
    }
    return ERROR;
}

int main(void) {
    struct Context ctx;
    ctx.current = START;
    ctx.num_count = 0;
    ctx.word_count = 0;
    reset_buffer(&ctx);
    
    printf("Enter text (alphanumeric only, max 1000 chars): ");
    
    int ch;
    int char_count = 0;
    while ((ch = getchar()) != EOF && char_count < 1000) {
        if (ch == '\n') {
            break;
        }
        
        char c = (char)ch;
        ctx.current = process_char(&ctx, c);
        
        if (ctx.current == ERROR) {
            printf("Error: Invalid input character '%c'\n", c);
            return 1;
        }
        
        char_count++;
    }
    
    if (ctx.current == READING_NUM) {
        ctx.num_count++;
    } else if (ctx.current == READING_WORD) {
        ctx.word_count++;
    }
    
    printf("Numbers: %d\n", ctx.num_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}
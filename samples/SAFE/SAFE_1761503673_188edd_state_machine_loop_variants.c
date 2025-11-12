//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    INIT,
    READ_CHAR,
    PROCESS_WORD,
    COUNT_DIGITS,
    FINALIZE,
    ERROR
};

struct context {
    enum state current;
    char buffer[256];
    size_t buf_pos;
    int word_count;
    int digit_count;
    int total_chars;
};

void state_init(struct context *ctx) {
    ctx->current = READ_CHAR;
    ctx->buf_pos = 0;
    ctx->word_count = 0;
    ctx->digit_count = 0;
    ctx->total_chars = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_read_char(struct context *ctx, int c) {
    if (c == EOF) {
        ctx->current = FINALIZE;
        return;
    }
    
    if (ctx->buf_pos >= sizeof(ctx->buffer) - 1) {
        ctx->current = ERROR;
        return;
    }
    
    ctx->buffer[ctx->buf_pos++] = (char)c;
    ctx->total_chars++;
    
    if (isspace(c)) {
        ctx->current = PROCESS_WORD;
    } else if (isdigit(c)) {
        ctx->current = COUNT_DIGITS;
    }
}

void state_process_word(struct context *ctx) {
    if (ctx->buf_pos > 0) {
        ctx->word_count++;
    }
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->current = READ_CHAR;
}

void state_count_digits(struct context *ctx) {
    if (ctx->buf_pos > 0 && isdigit(ctx->buffer[ctx->buf_pos - 1])) {
        ctx->digit_count++;
    }
    ctx->current = READ_CHAR;
}

void state_finalize(struct context *ctx) {
    if (ctx->buf_pos > 0) {
        ctx->word_count++;
        
        for (size_t i = 0; i < ctx->buf_pos; i++) {
            if (isdigit(ctx->buffer[i])) {
                ctx->digit_count++;
            }
        }
    }
    ctx->current = INIT;
}

void state_error(struct context *ctx) {
    fprintf(stderr, "Error: Buffer overflow\n");
    ctx->current = INIT;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (1) {
        int c = getchar();
        
        switch (ctx.current) {
            case INIT:
                state_init(&ctx);
                break;
            case READ_CHAR:
                state_read_char(&ctx, c);
                break;
            case PROCESS_WORD:
                state_process_word(&ctx);
                break;
            case COUNT_DIGITS:
                state_count_digits(&ctx);
                break;
            case FINALIZE:
                state_finalize(&ctx);
                printf("Words: %d\n", ctx.word_count);
                printf("Digits: %d\n", ctx.digit_count);
                printf("Total characters: %d\n", ctx.total_chars);
                return 0;
            case ERROR:
                state_error(&ctx);
                break;
        }
        
        if (ctx.current == INIT && c == EOF) {
            break;
        }
    }
    
    return 0;
}
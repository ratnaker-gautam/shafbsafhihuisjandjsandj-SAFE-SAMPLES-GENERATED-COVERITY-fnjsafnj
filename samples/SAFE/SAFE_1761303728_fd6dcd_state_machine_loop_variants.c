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
    char buffer[64];
    size_t buf_pos;
};

static void reset_buffer(struct context *ctx) {
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buf_pos = 0;
}

static int is_valid_number(const char *str) {
    if (str[0] == '\0') return 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}

static int is_valid_word(const char *str) {
    if (str[0] == '\0') return 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i])) return 0;
    }
    return 1;
}

static void process_token(struct context *ctx) {
    if (ctx->buf_pos > 0) {
        ctx->buffer[ctx->buf_pos] = '\0';
        if (is_valid_number(ctx->buffer)) {
            ctx->number_count++;
        } else if (is_valid_word(ctx->buffer)) {
            ctx->word_count++;
        }
        reset_buffer(ctx);
    }
}

int main(void) {
    struct context ctx = {START, 0, 0, {0}, 0};
    int c;
    
    while (ctx.current != FINISHED && ctx.current != ERROR) {
        switch (ctx.current) {
            case START:
                c = getchar();
                if (c == EOF) {
                    ctx.current = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    ctx.current = READING_NUMBER;
                    if (ctx.buf_pos < sizeof(ctx.buffer) - 1) {
                        ctx.buffer[ctx.buf_pos++] = (char)c;
                    }
                } else if (isalpha((unsigned char)c)) {
                    ctx.current = READING_WORD;
                    if (ctx.buf_pos < sizeof(ctx.buffer) - 1) {
                        ctx.buffer[ctx.buf_pos++] = (char)c;
                    }
                } else if (isspace((unsigned char)c)) {
                    ctx.current = SKIPPING_SPACE;
                } else {
                    ctx.current = ERROR;
                }
                break;
                
            case READING_NUMBER:
                c = getchar();
                if (c == EOF) {
                    process_token(&ctx);
                    ctx.current = FINISHED;
                } else if (isdigit((unsigned char)c)) {
                    if (ctx.buf_pos < sizeof(ctx.buffer) - 1) {
                        ctx.buffer[ctx.buf_pos++] = (char)c;
                    }
                } else if (isspace((unsigned char)c)) {
                    process_token(&ctx);
                    ctx.current = SKIPPING_SPACE;
                } else {
                    ctx.current = ERROR;
                }
                break;
                
            case READING_WORD:
                c = getchar();
                if (c == EOF) {
                    process_token(&ctx);
                    ctx.current = FINISHED;
                } else if (isalpha((unsigned char)c)) {
                    if (ctx.buf_pos < sizeof(ctx.buffer) - 1) {
                        ctx.buffer[ctx.buf_pos++] = (char)c;
                    }
                } else if (isspace((unsigned char)c)) {
                    process_token(&ctx);
                    ctx.current = SKIPPING_SPACE;
                } else {
                    ctx.current = ERROR;
                }
                break;
                
            case SKIPPING_SPACE:
                c = getchar();
                if (c == EOF) {
                    ctx.current = FINISHED;
                } else if (!isspace((unsigned char)c)) {
                    ungetc(c, stdin);
                    ctx.current = START;
                }
                break;
                
            case FINISHED:
            case ERROR:
                break;
        }
    }
    
    if (ctx.current == ERROR) {
        fprintf(stderr, "Error: Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    process_token(&ctx);
    printf("Numbers: %d\nWords: %d\n", ctx.number_count, ctx.word_count);
    return EXIT_SUCCESS;
}
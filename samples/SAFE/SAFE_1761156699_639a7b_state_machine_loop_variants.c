//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

struct context {
    enum state current;
    int number_count;
    int word_count;
    char buffer[64];
    size_t buf_pos;
};

void state_init(struct context *ctx) {
    ctx->current = INIT;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void process_char(struct context *ctx, char c) {
    switch (ctx->current) {
        case INIT:
            if (isspace(c)) {
                break;
            } else if (isdigit(c)) {
                ctx->current = READING_NUMBER;
                ctx->buffer[ctx->buf_pos++] = c;
            } else if (isalpha(c)) {
                ctx->current = READING_WORD;
                ctx->buffer[ctx->buf_pos++] = c;
            } else {
                ctx->current = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(c)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (isspace(c)) {
                ctx->number_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = INIT;
            } else {
                ctx->current = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalpha(c)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (isspace(c)) {
                ctx->word_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                ctx->current = INIT;
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
    state_init(&ctx);
    
    printf("Enter text (numbers and words, max 1000 chars): ");
    fflush(stdout);
    
    char input[1002];
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
    
    size_t i = 0;
    while (i < len && ctx.current != ERROR && ctx.current != DONE) {
        process_char(&ctx, input[i]);
        i++;
    }
    
    if (ctx.current == READING_NUMBER) {
        ctx.number_count++;
    } else if (ctx.current == READING_WORD) {
        ctx.word_count++;
    }
    
    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers: %d\n", ctx.number_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}
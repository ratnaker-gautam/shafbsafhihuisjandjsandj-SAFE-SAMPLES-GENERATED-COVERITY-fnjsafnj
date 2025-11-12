//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

enum state {
    INIT,
    READING_NUMBER,
    READING_WORD,
    ERROR,
    DONE
};

struct context {
    enum state current;
    int number;
    char word[32];
    size_t word_len;
    int sum;
    int word_count;
};

void state_init(struct context *ctx) {
    ctx->current = INIT;
    ctx->number = 0;
    ctx->word_len = 0;
    ctx->sum = 0;
    ctx->word_count = 0;
    memset(ctx->word, 0, sizeof(ctx->word));
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_char(struct context *ctx, char c) {
    if (!is_valid_char(c)) {
        ctx->current = ERROR;
        return;
    }

    switch (ctx->current) {
        case INIT:
            if (isdigit((unsigned char)c)) {
                ctx->current = READING_NUMBER;
                ctx->number = c - '0';
            } else if (isalpha((unsigned char)c)) {
                ctx->current = READING_WORD;
                ctx->word[0] = c;
                ctx->word_len = 1;
            } else if (c == '\n') {
                ctx->current = DONE;
            }
            break;

        case READING_NUMBER:
            if (isdigit((unsigned char)c)) {
                if (ctx->number <= (INT_MAX - (c - '0')) / 10) {
                    ctx->number = ctx->number * 10 + (c - '0');
                } else {
                    ctx->current = ERROR;
                }
            } else if (isalpha((unsigned char)c)) {
                ctx->sum += ctx->number;
                ctx->current = READING_WORD;
                ctx->word[0] = c;
                ctx->word_len = 1;
            } else if (isspace((unsigned char)c)) {
                ctx->sum += ctx->number;
                ctx->current = INIT;
            } else {
                ctx->current = ERROR;
            }
            break;

        case READING_WORD:
            if (isalpha((unsigned char)c)) {
                if (ctx->word_len < sizeof(ctx->word) - 1) {
                    ctx->word[ctx->word_len++] = c;
                } else {
                    ctx->current = ERROR;
                }
            } else if (isdigit((unsigned char)c)) {
                ctx->word[ctx->word_len] = '\0';
                ctx->word_count++;
                ctx->current = READING_NUMBER;
                ctx->number = c - '0';
            } else if (isspace((unsigned char)c)) {
                ctx->word[ctx->word_len] = '\0';
                ctx->word_count++;
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
    
    printf("Enter text (numbers and words): ");
    fflush(stdout);
    
    int c;
    while ((c = getchar()) != EOF && ctx.current != ERROR && ctx.current != DONE) {
        process_char(&ctx, (char)c);
    }
    
    if (ctx.current == READING_NUMBER) {
        ctx.sum += ctx.number;
        ctx.current = INIT;
    } else if (ctx.current == READING_WORD) {
        ctx.word[ctx.word_len] = '\0';
        ctx.word_count++;
        ctx.current = INIT;
    }
    
    if (ctx.current == ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Sum of numbers: %d\n", ctx.sum);
    printf("Word count: %d\n", ctx.word_count);
    
    return 0;
}
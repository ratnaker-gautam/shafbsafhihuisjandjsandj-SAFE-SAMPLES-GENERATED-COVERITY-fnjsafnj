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
    FINISHED,
    ERROR
};

struct context {
    enum state current;
    int number_count;
    int word_count;
    char buffer[256];
    size_t buf_pos;
};

void state_init(struct context *ctx) {
    ctx->current = INIT;
    ctx->number_count = 0;
    ctx->word_count = 0;
    ctx->buf_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_transition(struct context *ctx, char input) {
    switch(ctx->current) {
        case INIT:
            if (isdigit(input)) {
                ctx->current = READING_NUMBER;
                ctx->buffer[ctx->buf_pos++] = input;
            } else if (isalpha(input)) {
                ctx->current = READING_WORD;
                ctx->buffer[ctx->buf_pos++] = input;
            } else if (input == ';') {
                ctx->current = FINISHED;
            } else if (!isspace(input)) {
                ctx->current = ERROR;
            }
            break;
            
        case READING_NUMBER:
            if (isdigit(input)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = input;
                } else {
                    ctx->current = ERROR;
                }
            } else if (isalpha(input)) {
                ctx->current = ERROR;
            } else if (isspace(input) || input == ';') {
                ctx->number_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (input == ';') {
                    ctx->current = FINISHED;
                } else {
                    ctx->current = INIT;
                }
            } else {
                ctx->current = ERROR;
            }
            break;
            
        case READING_WORD:
            if (isalpha(input)) {
                if (ctx->buf_pos < sizeof(ctx->buffer) - 1) {
                    ctx->buffer[ctx->buf_pos++] = input;
                } else {
                    ctx->current = ERROR;
                }
            } else if (isdigit(input)) {
                ctx->current = ERROR;
            } else if (isspace(input) || input == ';') {
                ctx->word_count++;
                ctx->buf_pos = 0;
                memset(ctx->buffer, 0, sizeof(ctx->buffer));
                if (input == ';') {
                    ctx->current = FINISHED;
                } else {
                    ctx->current = INIT;
                }
            } else {
                ctx->current = ERROR;
            }
            break;
            
        case FINISHED:
        case ERROR:
            break;
    }
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    printf("Enter text (end with ';'): ");
    
    int ch;
    while ((ch = getchar()) != EOF && ctx.current != FINISHED && ctx.current != ERROR) {
        if (ch == '\n') {
            continue;
        }
        state_transition(&ctx, (char)ch);
    }
    
    if (ctx.current == ERROR) {
        printf("Error: Invalid input sequence\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", ctx.number_count);
    printf("Words found: %d\n", ctx.word_count);
    
    return 0;
}
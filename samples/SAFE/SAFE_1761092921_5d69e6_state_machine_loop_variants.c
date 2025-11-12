//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct Context {
    enum State current;
    char buffer[256];
    size_t pos;
    int word_count;
    int char_count;
};

void state_idle(struct Context *ctx, char input) {
    if (isalpha(input)) {
        ctx->current = READING;
        ctx->buffer[0] = input;
        ctx->pos = 1;
        ctx->char_count++;
    }
}

void state_reading(struct Context *ctx, char input) {
    if (isalpha(input)) {
        if (ctx->pos < 255) {
            ctx->buffer[ctx->pos++] = input;
            ctx->char_count++;
        } else {
            ctx->current = ERROR;
        }
    } else if (isspace(input)) {
        ctx->buffer[ctx->pos] = '\0';
        ctx->current = PROCESSING;
    } else {
        ctx->current = ERROR;
    }
}

void state_processing(struct Context *ctx) {
    if (ctx->pos > 0) {
        ctx->word_count++;
    }
    ctx->current = WRITING;
}

void state_writing(struct Context *ctx) {
    if (ctx->pos > 0) {
        printf("Word %d: %s\n", ctx->word_count, ctx->buffer);
    }
    ctx->pos = 0;
    ctx->current = IDLE;
}

void state_error(struct Context *ctx) {
    fprintf(stderr, "Error: Invalid input sequence\n");
    ctx->current = DONE;
}

int main(void) {
    struct Context ctx = { IDLE, {0}, 0, 0, 0 };
    int c;
    
    while (ctx.current != DONE) {
        switch (ctx.current) {
            case IDLE:
                c = getchar();
                if (c == EOF) {
                    ctx.current = DONE;
                } else if (c == '\n') {
                    continue;
                } else {
                    state_idle(&ctx, (char)c);
                }
                break;
                
            case READING:
                c = getchar();
                if (c == EOF) {
                    ctx.buffer[ctx.pos] = '\0';
                    ctx.current = PROCESSING;
                } else {
                    state_reading(&ctx, (char)c);
                }
                break;
                
            case PROCESSING:
                state_processing(&ctx);
                break;
                
            case WRITING:
                state_writing(&ctx);
                break;
                
            case ERROR:
                state_error(&ctx);
                break;
                
            case DONE:
                break;
        }
    }
    
    if (ctx.word_count > 0 || ctx.char_count > 0) {
        printf("Summary: %d words, %d characters\n", ctx.word_count, ctx.char_count);
    }
    
    return 0;
}
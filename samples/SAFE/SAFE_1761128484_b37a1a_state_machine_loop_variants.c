//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct context {
    enum state current;
    char buffer[256];
    size_t pos;
    int count;
};

int is_valid_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ';
}

void state_init(struct context *ctx) {
    ctx->current = READING;
    ctx->pos = 0;
    ctx->count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_reading(struct context *ctx) {
    int c = getchar();
    if (c == EOF) {
        ctx->current = DONE;
        return;
    }
    
    if (!is_valid_char((char)c)) {
        ctx->current = ERROR;
        return;
    }
    
    if (ctx->pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->pos++] = (char)c;
        if (c == '\n') {
            ctx->current = PROCESSING;
        }
    } else {
        ctx->current = ERROR;
    }
}

void state_processing(struct context *ctx) {
    size_t i = 0;
    while (i < ctx->pos) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
        i++;
    }
    ctx->current = OUTPUT;
}

void state_output(struct context *ctx) {
    size_t i = 0;
    while (i < ctx->pos) {
        putchar(ctx->buffer[i]);
        i++;
    }
    ctx->count++;
    ctx->current = READING;
    ctx->pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_error(struct context *ctx) {
    const char msg[] = "ERROR: Invalid input\n";
    size_t i = 0;
    while (i < sizeof(msg) - 1) {
        putchar(msg[i]);
        i++;
    }
    ctx->current = DONE;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (ctx.current != DONE) {
        switch (ctx.current) {
            case INIT:
                state_init(&ctx);
                break;
            case READING:
                state_reading(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case OUTPUT:
                state_output(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
            case DONE:
                break;
        }
    }
    
    printf("Processed %d lines\n", ctx.count);
    return 0;
}
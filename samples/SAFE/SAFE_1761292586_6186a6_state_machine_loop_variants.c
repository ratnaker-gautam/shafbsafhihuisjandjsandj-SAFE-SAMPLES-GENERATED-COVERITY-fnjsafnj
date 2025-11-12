//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int line_count;
};

void state_init(struct context *ctx) {
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->line_count = 0;
    ctx->current_state = READING;
}

void state_reading(struct context *ctx) {
    int c = getchar();
    
    if (c == EOF) {
        ctx->current_state = DONE;
        return;
    }
    
    if (c == '\n') {
        ctx->line_count++;
        ctx->current_state = PROCESSING;
        return;
    }
    
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = (char)c;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_processing(struct context *ctx) {
    if (ctx->buffer_pos == 0) {
        ctx->current_state = OUTPUT;
        return;
    }
    
    int in_word = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (isalpha((unsigned char)ctx->buffer[i])) {
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    ctx->current_state = OUTPUT;
}

void state_output(struct context *ctx) {
    if (ctx->buffer_pos > 0) {
        printf("Line %d: %d words\n", ctx->line_count, ctx->word_count);
    }
    
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->current_state = READING;
}

void state_error(struct context *ctx) {
    fprintf(stderr, "Error: Buffer overflow\n");
    ctx->current_state = DONE;
}

int main(void) {
    struct context ctx;
    state_init(&ctx);
    
    while (1) {
        switch (ctx.current_state) {
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
                goto done;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
done:
    if (ctx.line_count > 0) {
        printf("Total lines: %d\n", ctx.line_count);
    }
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int line_count;
};

void state_init(struct Context *ctx) {
    ctx->current_state = READING;
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->line_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_reading(struct Context *ctx) {
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

void state_processing(struct Context *ctx) {
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

void state_output(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        printf("Line %d: ", ctx->line_count);
        for (size_t i = 0; i < ctx->buffer_pos; i++) {
            putchar(ctx->buffer[i]);
        }
        printf(" (words: %d)\n", ctx->word_count);
    }
    
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->current_state = READING;
}

void state_error(struct Context *ctx) {
    fprintf(stderr, "Error: Buffer overflow\n");
    ctx->current_state = DONE;
}

void state_done(struct Context *ctx) {
    printf("Processing complete. Total lines: %d\n", ctx->line_count);
}

int main(void) {
    struct Context ctx;
    state_init(&ctx);
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
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
                state_done(&ctx);
                break;
        }
    }
    
    if (ctx.current_state == ERROR) {
        state_error(&ctx);
    } else if (ctx.current_state == DONE) {
        state_done(&ctx);
    }
    
    return ctx.current_state == ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
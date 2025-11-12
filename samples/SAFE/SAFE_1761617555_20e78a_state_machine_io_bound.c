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
    int char_count;
    int line_count;
};

void init_context(struct Context *ctx) {
    ctx->current_state = INIT;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    ctx->line_count = 0;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void state_init(struct Context *ctx) {
    ctx->current_state = READING;
}

void state_reading(struct Context *ctx) {
    int c = getchar();
    
    if (c == EOF) {
        ctx->current_state = DONE;
        return;
    }
    
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer[ctx->buffer_pos++] = (char)c;
    ctx->char_count++;
    
    if (c == '\n') {
        ctx->line_count++;
    }
    
    if (is_whitespace(c)) {
        ctx->current_state = PROCESSING;
    }
}

void state_processing(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        int in_word = 0;
        for (size_t i = 0; i < ctx->buffer_pos; i++) {
            if (!is_whitespace(ctx->buffer[i])) {
                if (!in_word) {
                    ctx->word_count++;
                    in_word = 1;
                }
            } else {
                in_word = 0;
            }
        }
    }
    
    ctx->buffer_pos = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->current_state = OUTPUT;
}

void state_output(struct Context *ctx) {
    printf("Words: %d, Characters: %d, Lines: %d\n", 
           ctx->word_count, ctx->char_count, ctx->line_count);
    ctx->current_state = READING;
}

void state_error(struct Context *ctx) {
    fprintf(stderr, "Error: Buffer overflow detected\n");
    ctx->current_state = DONE;
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);
    
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
                break;
        }
    }
    
    if (ctx.current_state == DONE) {
        if (ctx.buffer_pos > 0) {
            state_processing(&ctx);
            printf("Final count - Words: %d, Characters: %d, Lines: %d\n", 
                   ctx.word_count, ctx.char_count, ctx.line_count);
        } else {
            printf("Final count - Words: %d, Characters: %d, Lines: %d\n", 
                   ctx.word_count, ctx.char_count, ctx.line_count);
        }
    }
    
    return ctx.current_state == ERROR ? 1 : 0;
}
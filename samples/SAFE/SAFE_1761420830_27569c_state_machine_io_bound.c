//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    IDLE,
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

void state_idle(struct context *ctx, int ch);
void state_reading(struct context *ctx, int ch);
void state_processing(struct context *ctx, int ch);
void state_output(struct context *ctx, int ch);
void state_error(struct context *ctx, int ch);
void state_done(struct context *ctx, int ch);

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.line_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));

    printf("Enter text (Ctrl+D to end):\n");

    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        int ch = getchar();
        
        if (ch == EOF) {
            ch = '\n';
        }

        switch (ctx.current_state) {
            case IDLE:
                state_idle(&ctx, ch);
                break;
            case READING:
                state_reading(&ctx, ch);
                break;
            case PROCESSING:
                state_processing(&ctx, ch);
                break;
            case OUTPUT:
                state_output(&ctx, ch);
                break;
            case ERROR:
                state_error(&ctx, ch);
                break;
            case DONE:
                state_done(&ctx, ch);
                break;
        }
    }

    if (ctx.current_state == ERROR) {
        fprintf(stderr, "Error occurred during processing\n");
        return 1;
    }

    printf("Processing complete:\n");
    printf("Lines: %d\n", ctx.line_count);
    printf("Words: %d\n", ctx.word_count);
    
    return 0;
}

void state_idle(struct context *ctx, int ch) {
    if (ch == EOF) {
        ctx->current_state = DONE;
        return;
    }
    
    if (isalpha(ch) || isdigit(ch)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)ch;
            ctx->current_state = READING;
        } else {
            ctx->current_state = ERROR;
        }
    } else if (ch == '\n') {
        ctx->line_count++;
    }
}

void state_reading(struct context *ctx, int ch) {
    if (ch == EOF) {
        ctx->current_state = PROCESSING;
        return;
    }
    
    if (isalpha(ch) || isdigit(ch)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)ch;
        } else {
            ctx->current_state = ERROR;
        }
    } else {
        ctx->current_state = PROCESSING;
    }
}

void state_processing(struct context *ctx, int ch) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->word_count++;
        ctx->buffer_pos = 0;
    }
    
    if (ch == '\n') {
        ctx->line_count++;
    }
    
    if (ch == EOF) {
        ctx->current_state = OUTPUT;
    } else if (isalpha(ch) || isdigit(ch)) {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = (char)ch;
            ctx->current_state = READING;
        } else {
            ctx->current_state = ERROR;
        }
    } else {
        ctx->current_state = IDLE;
    }
}

void state_output(struct context *ctx, int ch) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->word_count++;
        ctx->buffer_pos = 0;
    }
    ctx->current_state = DONE;
}

void state_error(struct context *ctx, int ch) {
    return;
}

void state_done(struct context *ctx, int ch) {
    return;
}
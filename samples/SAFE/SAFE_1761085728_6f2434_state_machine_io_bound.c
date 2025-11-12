//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int line_count;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->line_count = 0;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

enum State handle_idle(struct Context *ctx) {
    printf("Enter text (empty line to finish):\n");
    return STATE_READING;
}

enum State handle_reading(struct Context *ctx) {
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strlen(ctx->buffer) == 0) {
        return STATE_DONE;
    }
    
    ctx->line_count++;
    return STATE_PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    char *ptr = ctx->buffer;
    int in_word = 0;
    
    while (*ptr != '\0') {
        if (is_whitespace(*ptr)) {
            in_word = 0;
        } else {
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        }
        ptr++;
    }
    
    return STATE_WRITING;
}

enum State handle_writing(struct Context *ctx) {
    printf("Processed line: \"%s\"\n", ctx->buffer);
    printf("Current stats: %d lines, %d words\n", ctx->line_count, ctx->word_count);
    return STATE_READING;
}

enum State handle_error(struct Context *ctx) {
    printf("Error occurred during processing\n");
    return STATE_DONE;
}

enum State handle_done(struct Context *ctx) {
    printf("Final statistics: %d lines, %d words\n", ctx->line_count, ctx->word_count);
    return STATE_DONE;
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    while (ctx.current_state != STATE_DONE && ctx.current_state != STATE_ERROR) {
        switch (ctx.current_state) {
            case STATE_IDLE:
                ctx.current_state = handle_idle(&ctx);
                break;
            case STATE_READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case STATE_PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case STATE_WRITING:
                ctx.current_state = handle_writing(&ctx);
                break;
            case STATE_ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case STATE_DONE:
                ctx.current_state = handle_done(&ctx);
                break;
            default:
                ctx.current_state = STATE_ERROR;
                break;
        }
        
        if (ctx.line_count > 1000 || ctx.word_count > 10000) {
            ctx.current_state = STATE_ERROR;
        }
    }
    
    return ctx.current_state == STATE_ERROR ? EXIT_FAILURE : EXIT_SUCCESS;
}
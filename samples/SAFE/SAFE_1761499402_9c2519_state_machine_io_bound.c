//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int char_count;
    int line_count;
};

void init_context(struct context *ctx) {
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

int handle_init(struct context *ctx) {
    printf("Enter text (empty line to finish):\n");
    ctx->current_state = READING;
    return 0;
}

int handle_reading(struct context *ctx) {
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = DONE;
        return 0;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        ctx->current_state = DONE;
        return 0;
    }
    
    ctx->current_state = PROCESSING;
    ctx->buffer_pos = 0;
    ctx->line_count++;
    return 0;
}

int handle_processing(struct context *ctx) {
    int in_word = 0;
    
    while (ctx->buffer[ctx->buffer_pos] != '\0') {
        char c = ctx->buffer[ctx->buffer_pos];
        ctx->char_count++;
        
        if (is_whitespace(c)) {
            if (in_word) {
                ctx->word_count++;
                in_word = 0;
            }
        } else {
            if (!in_word) {
                in_word = 1;
            }
        }
        
        ctx->buffer_pos++;
    }
    
    if (in_word) {
        ctx->word_count++;
    }
    
    ctx->current_state = OUTPUT;
    return 0;
}

int handle_output(struct context *ctx) {
    printf("Line %d processed: %s\n", ctx->line_count, ctx->buffer);
    ctx->current_state = READING;
    return 0;
}

int handle_error(struct context *ctx) {
    printf("Error occurred during processing\n");
    ctx->current_state = DONE;
    return 1;
}

int handle_done(struct context *ctx) {
    printf("\nStatistics:\n");
    printf("Lines: %d\n", ctx->line_count);
    printf("Words: %d\n", ctx->word_count);
    printf("Characters: %d\n", ctx->char_count);
    return 0;
}

int main(void) {
    struct context ctx;
    init_context(&ctx);
    int result = 0;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case INIT:
                result = handle_init(&ctx);
                break;
            case READING:
                result = handle_reading(&ctx);
                break;
            case PROCESSING:
                result = handle_processing(&ctx);
                break;
            case OUTPUT:
                result = handle_output(&ctx);
                break;
            case ERROR:
                result = handle_error(&ctx);
                break;
            case DONE:
                result = handle_done(&ctx);
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
        
        if (result != 0) {
            ctx.current_state = ERROR;
        }
    }
    
    if (ctx.current_state == ERROR) {
        handle_error(&ctx);
        return 1;
    }
    
    handle_done(&ctx);
    return 0;
}
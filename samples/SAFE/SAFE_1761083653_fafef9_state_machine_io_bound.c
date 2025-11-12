//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
};

void state_machine_init(struct context *ctx) {
    ctx->current_state = STATE_START;
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

enum state handle_start(struct context *ctx) {
    printf("Enter text (max 255 chars, empty line to finish):\n");
    return STATE_READING;
}

enum state handle_reading(struct context *ctx) {
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return STATE_DONE;
    }
    
    ctx->buffer_pos = 0;
    return STATE_PROCESSING;
}

enum state handle_processing(struct context *ctx) {
    int in_word = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    
    for (size_t i = 0; i < strlen(ctx->buffer); i++) {
        char c = ctx->buffer[i];
        
        if (isalpha((unsigned char)c)) {
            ctx->char_count++;
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    return STATE_OUTPUT;
}

enum state handle_output(struct context *ctx) {
    printf("Words: %d, Characters: %d\n", ctx->word_count, ctx->char_count);
    return STATE_READING;
}

enum state handle_error(struct context *ctx) {
    printf("Error reading input\n");
    return STATE_DONE;
}

enum state handle_done(struct context *ctx) {
    printf("Processing complete\n");
    return STATE_DONE;
}

int main(void) {
    struct context ctx;
    state_machine_init(&ctx);
    
    while (ctx.current_state != STATE_DONE) {
        switch (ctx.current_state) {
            case STATE_START:
                ctx.current_state = handle_start(&ctx);
                break;
            case STATE_READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case STATE_PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case STATE_OUTPUT:
                ctx.current_state = handle_output(&ctx);
                break;
            case STATE_ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case STATE_DONE:
                break;
        }
        
        if (ctx.current_state == STATE_ERROR) {
            break;
        }
    }
    
    return 0;
}
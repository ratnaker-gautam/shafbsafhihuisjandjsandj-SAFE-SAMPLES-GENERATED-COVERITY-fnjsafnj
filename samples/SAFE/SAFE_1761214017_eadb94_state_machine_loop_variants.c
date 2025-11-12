//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_pos;
    int processed_count;
    int error_code;
};

void state_machine_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->processed_count = 0;
    ctx->error_code = 0;
}

enum state handle_idle(struct context *ctx) {
    printf("Enter text to process (empty line to finish): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strlen(ctx->buffer) == 0) {
        return STATE_DONE;
    }
    
    ctx->buffer_pos = 0;
    return STATE_READING;
}

enum state handle_reading(struct context *ctx) {
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        ctx->error_code = 2;
        return STATE_ERROR;
    }
    
    if (ctx->buffer[ctx->buffer_pos] == '\0') {
        return STATE_PROCESSING;
    }
    
    ctx->buffer_pos++;
    return STATE_READING;
}

enum state handle_processing(struct context *ctx) {
    size_t i = 0;
    while (i < sizeof(ctx->buffer) && ctx->buffer[i] != '\0') {
        if (isalpha((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = toupper((unsigned char)ctx->buffer[i]);
            ctx->processed_count++;
        }
        i++;
    }
    return STATE_WRITING;
}

enum state handle_writing(struct context *ctx) {
    printf("Processed: %s\n", ctx->buffer);
    printf("Characters processed: %d\n", ctx->processed_count);
    ctx->processed_count = 0;
    return STATE_IDLE;
}

enum state handle_error(struct context *ctx) {
    printf("Error %d occurred\n", ctx->error_code);
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
                break;
        }
    }
    
    return 0;
}
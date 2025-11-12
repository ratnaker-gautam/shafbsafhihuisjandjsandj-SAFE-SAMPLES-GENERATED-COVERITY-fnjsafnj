//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    size_t buffer_len;
    int line_count;
    int max_lines;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) {
            return 0;
        }
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void state_init(struct Context* ctx) {
    ctx->current_state = READING;
    ctx->buffer_len = 0;
    ctx->line_count = 0;
    ctx->max_lines = 5;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_reading(struct Context* ctx) {
    printf("Enter text (max 255 chars): ");
    fflush(stdout);
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = 0;
        len--;
    }
    
    if (!validate_input(ctx->buffer, len)) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer_len = len;
    ctx->current_state = PROCESSING;
}

void state_processing(struct Context* ctx) {
    if (ctx->buffer_len == 0) {
        ctx->current_state = DONE;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->current_state = OUTPUT;
}

void state_output(struct Context* ctx) {
    printf("Processed: %s\n", ctx->buffer);
    ctx->line_count++;
    
    if (ctx->line_count >= ctx->max_lines) {
        ctx->current_state = DONE;
    } else {
        ctx->current_state = READING;
    }
}

void state_error(struct Context* ctx) {
    printf("Error: Invalid input detected\n");
    ctx->current_state = DONE;
}

int main(void) {
    struct Context ctx;
    state_init(&ctx);
    
    while (ctx.current_state != DONE) {
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
    
    printf("Processing complete. Processed %d lines.\n", ctx.line_count);
    return 0;
}
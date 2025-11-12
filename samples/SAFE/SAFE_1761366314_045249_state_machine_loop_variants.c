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
    enum state current_state;
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
    }
    return 1;
}

void state_init(struct context* ctx) {
    ctx->current_state = READING;
    ctx->buffer_len = 0;
    ctx->line_count = 0;
    ctx->max_lines = 5;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

void state_reading(struct context* ctx) {
    printf("Enter text (empty line to finish): ");
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
    
    if (len == 0) {
        ctx->current_state = DONE;
    } else {
        ctx->current_state = PROCESSING;
    }
}

void state_processing(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->current_state = OUTPUT;
}

void state_output(struct context* ctx) {
    printf("Processed: %s\n", ctx->buffer);
    ctx->line_count++;
    
    if (ctx->line_count >= ctx->max_lines) {
        ctx->current_state = DONE;
    } else {
        ctx->current_state = READING;
    }
}

void state_error(struct context* ctx) {
    printf("Error occurred during processing\n");
    ctx->current_state = DONE;
}

void state_done(struct context* ctx) {
    printf("Processing complete. Lines processed: %d\n", ctx->line_count);
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
                state_done(&ctx);
                return 0;
        }
    }
    
    return 0;
}
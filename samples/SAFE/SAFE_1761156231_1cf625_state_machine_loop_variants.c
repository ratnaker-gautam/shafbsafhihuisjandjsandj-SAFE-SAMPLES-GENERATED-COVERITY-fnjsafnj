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
    printf("Enter text (max 255 chars): ");
    
    char input[512];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (len < sizeof(ctx->buffer)) {
        strncpy(ctx->buffer, input, sizeof(ctx->buffer)-1);
        ctx->buffer[sizeof(ctx->buffer)-1] = '\0';
        ctx->buffer_len = len;
        ctx->current_state = PROCESSING;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_processing(struct context* ctx) {
    int uppercase_count = 0;
    int digit_count = 0;
    
    for (size_t i = 0; i < ctx->buffer_len && i < sizeof(ctx->buffer); i++) {
        char c = ctx->buffer[i];
        if (c >= 'A' && c <= 'Z') {
            uppercase_count++;
        } else if (c >= '0' && c <= '9') {
            digit_count++;
        }
    }
    
    printf("Processing: %zu chars, %d uppercase, %d digits\n", 
           ctx->buffer_len, uppercase_count, digit_count);
    
    ctx->current_state = OUTPUT;
}

void state_output(struct context* ctx) {
    printf("Output: '%.*s'\n", (int)ctx->buffer_len, ctx->buffer);
    ctx->line_count++;
    
    if (ctx->line_count >= ctx->max_lines) {
        ctx->current_state = DONE;
    } else {
        ctx->current_state = READING;
    }
}

void state_error(struct context* ctx) {
    printf("Error: Invalid input detected\n");
    ctx->current_state = DONE;
}

void state_done(struct context* ctx) {
    printf("Completed %d lines\n", ctx->line_count);
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
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
    return 0;
}
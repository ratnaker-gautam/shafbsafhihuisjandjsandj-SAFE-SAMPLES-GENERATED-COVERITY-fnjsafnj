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
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_iterations;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_buffer(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
}

enum state handle_init(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    ctx->current_state = READING;
    ctx->buffer_len = 0;
    ctx->processed_count = 0;
    ctx->max_iterations = 5;
    return READING;
}

enum state handle_reading(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("Enter text (max 255 chars): ");
    fflush(stdout);
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return ERROR;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(ctx->buffer, len)) {
        return ERROR;
    }
    
    ctx->buffer_len = len;
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return ERROR;
    }
    
    process_buffer(ctx);
    ctx->processed_count++;
    return OUTPUT;
}

enum state handle_output(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("Processed: %s\n", ctx->buffer);
    printf("Total processed: %d\n", ctx->processed_count);
    
    if (ctx->processed_count >= ctx->max_iterations) {
        return DONE;
    }
    
    return READING;
}

enum state handle_error(struct context* ctx) {
    if (ctx != NULL) {
        printf("Error occurred in state machine\n");
    }
    return DONE;
}

int main(void) {
    struct context ctx;
    enum state next_state;
    
    ctx.current_state = INIT;
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                next_state = handle_init(&ctx);
                break;
            case READING:
                next_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                next_state = handle_processing(&ctx);
                break;
            case OUTPUT:
                next_state = handle_output(&ctx);
                break;
            case ERROR:
                next_state = handle_error(&ctx);
                break;
            case DONE:
                goto exit_loop;
            default:
                next_state = ERROR;
                break;
        }
        
        if (next_state == ERROR || next_state == DONE) {
            ctx.current_state = next_state;
            if (next_state == DONE) {
                break;
            }
        } else {
            ctx.current_state = next_state;
        }
    }
    
exit_loop:
    printf("State machine completed\n");
    return 0;
}
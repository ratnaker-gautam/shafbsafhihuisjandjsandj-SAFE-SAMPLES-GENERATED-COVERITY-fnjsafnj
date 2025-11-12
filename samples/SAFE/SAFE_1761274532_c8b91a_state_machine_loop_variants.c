//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    COMPLETE,
    ERROR,
    EXIT
};

struct context {
    enum state current;
    int counter;
    char buffer[64];
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 64) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

enum state handle_init(struct context* ctx) {
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return READY;
}

enum state handle_ready(struct context* ctx) {
    printf("Enter command (process/exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) return ERROR;
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len-1] == '\n') ctx->buffer[--len] = '\0';
    if (!validate_input(ctx->buffer, len)) return ERROR;
    
    if (strcmp(ctx->buffer, "process") == 0) return PROCESSING;
    if (strcmp(ctx->buffer, "exit") == 0) return EXIT;
    return ERROR;
}

enum state handle_processing(struct context* ctx) {
    if (ctx->counter < 0) return ERROR;
    if (ctx->counter > 1000) return COMPLETE;
    
    int i = 0;
    do {
        ctx->counter++;
        i++;
    } while (i < 10 && ctx->counter <= 1000);
    
    printf("Processing... %d/1000\n", ctx->counter);
    return (ctx->counter >= 1000) ? COMPLETE : PROCESSING;
}

enum state handle_complete(struct context* ctx) {
    printf("Processing complete! Counter: %d\n", ctx->counter);
    return READY;
}

enum state handle_error(struct context* ctx) {
    printf("Error occurred. Resetting.\n");
    return INIT;
}

int main(void) {
    struct context ctx = {INIT, 0, {0}};
    int running = 1;
    
    while (running) {
        switch (ctx.current) {
            case INIT:
                ctx.current = handle_init(&ctx);
                break;
            case READY:
                ctx.current = handle_ready(&ctx);
                break;
            case PROCESSING:
                ctx.current = handle_processing(&ctx);
                break;
            case COMPLETE:
                ctx.current = handle_complete(&ctx);
                break;
            case ERROR:
                ctx.current = handle_error(&ctx);
                break;
            case EXIT:
                running = 0;
                break;
            default:
                ctx.current = ERROR;
                break;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}
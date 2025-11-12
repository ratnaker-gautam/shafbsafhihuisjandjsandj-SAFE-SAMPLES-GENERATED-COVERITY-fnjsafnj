//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    FINALIZING,
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    int data_value;
    int step_count;
    char buffer[64];
};

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 63) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

enum state handle_init(struct context* ctx) {
    ctx->data_value = 0;
    ctx->step_count = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return READY;
}

enum state handle_ready(struct context* ctx, const char* input) {
    if (!validate_input(input)) return ERROR;
    strncpy(ctx->buffer, input, sizeof(ctx->buffer) - 1);
    ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    int value = atoi(ctx->buffer);
    if (value < 0 || value > 10000) return ERROR;
    
    int i = 0;
    while (i < 5) {
        if (value % 2 == 0) {
            value /= 2;
        } else {
            value = value * 3 + 1;
        }
        i++;
    }
    
    ctx->data_value = value;
    ctx->step_count += 5;
    return FINALIZING;
}

enum state handle_finalizing(struct context* ctx) {
    int result = 0;
    for (int i = 0; i < 3; i++) {
        result += ctx->data_value * (i + 1);
        if (result < 0) return ERROR;
    }
    ctx->data_value = result;
    return DONE;
}

enum state handle_error(struct context* ctx) {
    ctx->data_value = -1;
    return DONE;
}

int main(void) {
    struct context ctx;
    char input[128];
    
    ctx.current_state = INIT;
    
    while (ctx.current_state != DONE) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
                
            case READY:
                printf("Enter a number (0-10000): ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                }
                ctx.current_state = handle_ready(&ctx, input);
                break;
                
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
                
            case FINALIZING:
                ctx.current_state = handle_finalizing(&ctx);
                break;
                
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
                
            case DONE:
                break;
        }
    }
    
    if (ctx.data_value >= 0) {
        printf("Final result: %d (after %d steps)\n", ctx.data_value, ctx.step_count);
    } else {
        printf("Error occurred during processing\n");
    }
    
    return 0;
}
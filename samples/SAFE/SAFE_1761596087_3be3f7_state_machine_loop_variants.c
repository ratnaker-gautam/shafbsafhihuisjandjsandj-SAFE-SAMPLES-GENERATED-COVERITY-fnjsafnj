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
    strcpy(ctx->buffer, "");
    return READY;
}

enum state handle_ready(struct context* ctx, const char* input) {
    if (!validate_input(input)) return ERROR;
    if (strlen(input) > 63) return ERROR;
    strcpy(ctx->buffer, input);
    ctx->data_value = atoi(input);
    if (ctx->data_value < 0) return ERROR;
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    if (ctx->data_value <= 0) return ERROR;
    int i = 0;
    while (i < 5) {
        if (ctx->data_value > 1000000) return ERROR;
        ctx->data_value *= 2;
        ctx->step_count++;
        i++;
    }
    return FINALIZING;
}

enum state handle_finalizing(struct context* ctx) {
    int temp = ctx->data_value;
    int sum = 0;
    for (int i = 0; i < 100 && temp > 0; i++) {
        sum += temp % 10;
        temp /= 10;
        if (sum > 10000) break;
    }
    ctx->data_value = sum;
    return DONE;
}

enum state handle_done(struct context* ctx) {
    printf("Processing complete. Steps: %d, Result: %d\n", ctx->step_count, ctx->data_value);
    return DONE;
}

enum state handle_error(struct context* ctx) {
    printf("Error occurred during processing.\n");
    return ERROR;
}

int main(void) {
    struct context ctx;
    ctx.current_state = INIT;
    char input[64];
    
    while (1) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
            case READY:
                printf("Enter a positive number: ");
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') input[len-1] = '\0';
                ctx.current_state = handle_ready(&ctx, input);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case FINALIZING:
                ctx.current_state = handle_finalizing(&ctx);
                break;
            case DONE:
                handle_done(&ctx);
                return 0;
            case ERROR:
                handle_error(&ctx);
                return 1;
        }
    }
    
    return 0;
}
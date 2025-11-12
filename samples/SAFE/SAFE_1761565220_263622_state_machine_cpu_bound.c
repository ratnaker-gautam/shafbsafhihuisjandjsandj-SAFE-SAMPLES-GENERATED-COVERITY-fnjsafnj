//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

enum state {
    INIT,
    PROCESSING,
    VALIDATING,
    FINALIZING,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    uint32_t data[256];
    size_t data_count;
    uint32_t checksum;
    uint32_t iterations;
};

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 1024) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static void init_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = INIT;
    memset(ctx->data, 0, sizeof(ctx->data));
    ctx->data_count = 0;
    ctx->checksum = 0;
    ctx->iterations = 0;
}

static uint32_t compute_checksum(const uint32_t *data, size_t count) {
    if (data == NULL || count == 0) return 0;
    uint32_t sum = 0;
    for (size_t i = 0; i < count; i++) {
        if (sum > UINT32_MAX - data[i]) {
            sum = UINT32_MAX;
            break;
        }
        sum += data[i];
    }
    return sum;
}

static void process_data(struct context *ctx, const char *input) {
    if (ctx == NULL || input == NULL) return;
    size_t len = strlen(input);
    for (size_t i = 0; i < len && ctx->data_count < 256; i++) {
        uint32_t value = (uint32_t)(input[i] - '0');
        ctx->data[ctx->data_count++] = value;
    }
}

static void state_machine_step(struct context *ctx, const char *input) {
    if (ctx == NULL) return;
    
    switch (ctx->current_state) {
        case INIT:
            if (validate_input(input)) {
                ctx->current_state = PROCESSING;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case PROCESSING:
            process_data(ctx, input);
            ctx->current_state = VALIDATING;
            break;
            
        case VALIDATING:
            ctx->checksum = compute_checksum(ctx->data, ctx->data_count);
            if (ctx->checksum > 0 && ctx->data_count > 0) {
                ctx->current_state = FINALIZING;
            } else {
                ctx->current_state = ERROR;
            }
            break;
            
        case FINALIZING:
            for (size_t i = 0; i < ctx->data_count; i++) {
                for (uint32_t j = 0; j < 1000; j++) {
                    ctx->data[i] = (ctx->data[i] * 1103515245 + 12345) & 0x7FFFFFFF;
                }
            }
            ctx->iterations++;
            ctx->current_state = DONE;
            break;
            
        case ERROR:
            break;
            
        case DONE:
            break;
    }
}

int main(void) {
    struct context ctx;
    char input_buffer[1025];
    
    init_context(&ctx);
    
    printf("Enter numeric data (digits only, max 1024 chars): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        state_machine_step(&ctx, input_buffer);
    }
    
    if (ctx.current_state == ERROR) {
        printf("Processing error occurred\n");
        return 1;
    }
    
    printf("Processing completed successfully\n");
    printf("Processed %zu data elements\n", ctx.data_count);
    printf("Initial checksum: %u\n", ctx.checksum);
    printf("Iterations: %u\n", ctx.iterations);
    
    uint32_t final_checksum = compute_checksum(ctx.data, ctx.data_count);
    printf("Final checksum: %u\n", final_checksum);
    
    return 0;
}
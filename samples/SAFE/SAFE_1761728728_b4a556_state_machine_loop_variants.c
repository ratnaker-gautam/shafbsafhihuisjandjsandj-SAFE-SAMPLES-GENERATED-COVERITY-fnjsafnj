//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int data_count;
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

void process_data(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
}

enum state handle_idle(struct context* ctx) {
    printf("Enter data (max 255 chars): ");
    
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        printf("Invalid input\n");
        return ERROR;
    }
    
    if (len == 0) {
        return DONE;
    }
    
    memcpy(ctx->buffer, input, len);
    ctx->buffer[len] = '\0';
    ctx->buffer_len = len;
    
    return READING;
}

enum state handle_reading(struct context* ctx) {
    if (ctx->buffer_len == 0) {
        return ERROR;
    }
    
    printf("Read: %s\n", ctx->buffer);
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    process_data(ctx);
    printf("Processed: %s\n", ctx->buffer);
    ctx->data_count++;
    return WRITING;
}

enum state handle_writing(struct context* ctx) {
    printf("Writing completed for item %d\n", ctx->data_count);
    
    if (ctx->data_count >= ctx->max_iterations) {
        return DONE;
    }
    
    return IDLE;
}

enum state handle_error(struct context* ctx) {
    printf("Error state reached\n");
    ctx->data_count = 0;
    ctx->buffer_len = 0;
    return IDLE;
}

enum state handle_done(struct context* ctx) {
    printf("Processing complete. Total items: %d\n", ctx->data_count);
    return DONE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_iterations = 5;
    
    int iteration_count = 0;
    const int max_total_iterations = 100;
    
    while (ctx.current_state != DONE && iteration_count < max_total_iterations) {
        iteration_count++;
        
        switch (ctx.current_state) {
            case IDLE:
                ctx.current_state = handle_idle(&ctx);
                break;
            case READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case WRITING:
                ctx.current_state = handle_writing(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case DONE:
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
    if (iteration_count >= max_total_iterations) {
        printf("Maximum iterations reached\n");
    }
    
    return 0;
}
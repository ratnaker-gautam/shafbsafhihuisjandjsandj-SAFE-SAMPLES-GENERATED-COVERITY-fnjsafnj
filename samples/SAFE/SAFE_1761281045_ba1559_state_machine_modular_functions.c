//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    int processed_count;
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

int handle_idle(struct context* ctx, const char* input, size_t len) {
    if (ctx == NULL || input == NULL) {
        return ERROR;
    }
    
    if (!validate_input(input, len)) {
        return ERROR;
    }
    
    if (len > 0) {
        if (len < sizeof(ctx->buffer)) {
            memcpy(ctx->buffer, input, len);
            ctx->buffer_len = len;
            return READING;
        }
    }
    return ERROR;
}

int handle_reading(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return ERROR;
    }
    
    if (ctx->buffer_len > 0 && ctx->buffer_len < sizeof(ctx->buffer)) {
        return PROCESSING;
    }
    return ERROR;
}

int handle_processing(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return ERROR;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->processed_count++;
    
    if (ctx->processed_count < 3) {
        return WRITING;
    }
    return DONE;
}

int handle_writing(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return ERROR;
    }
    
    if (fwrite(ctx->buffer, 1, ctx->buffer_len, stdout) != ctx->buffer_len) {
        return ERROR;
    }
    if (fputc('\n', stdout) == EOF) {
        return ERROR;
    }
    
    ctx->buffer_len = 0;
    return IDLE;
}

int handle_error(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    const char* error_msg = "ERROR: Invalid state transition\n";
    if (fwrite(error_msg, 1, strlen(error_msg), stderr) != strlen(error_msg)) {
        return ERROR;
    }
    ctx->buffer_len = 0;
    return DONE;
}

void state_machine_step(struct context* ctx, const char* input, size_t len) {
    if (ctx == NULL) {
        return;
    }
    
    enum state next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case IDLE:
            next_state = handle_idle(ctx, input, len);
            break;
        case READING:
            next_state = handle_reading(ctx);
            break;
        case PROCESSING:
            next_state = handle_processing(ctx);
            break;
        case WRITING:
            next_state = handle_writing(ctx);
            break;
        case ERROR:
            next_state = handle_error(ctx);
            break;
        case DONE:
            return;
    }
    
    ctx->current_state = next_state;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    
    const char* test_inputs[] = {
        "hello world",
        "state machine",
        "final input"
    };
    const size_t num_inputs = sizeof(test_inputs) / sizeof(test_inputs[0]);
    
    for (size_t i = 0; i < num_inputs && ctx.current_state != DONE && ctx.current_state != ERROR; i++) {
        const char* input = test_inputs[i];
        size_t len = strlen(input);
        
        state_machine_step(&ctx, input, len);
        
        while (ctx.current_state != IDLE && ctx.current_state != DONE && ctx.current_state != ERROR) {
            state_machine_step(&ctx, NULL, 0);
        }
    }
    
    if (ctx.current_state == ERROR) {
        handle_error(&ctx);
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
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
    size_t buffer_pos;
    int processed_count;
    int error_code;
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
    if (ctx == NULL || ctx->buffer_pos == 0) {
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->processed_count++;
}

enum state handle_idle(struct context* ctx) {
    printf("Enter text (empty line to finish): ");
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
    
    if (len == 0) {
        return DONE;
    }
    
    ctx->buffer_pos = len;
    return READING;
}

enum state handle_reading(struct context* ctx) {
    if (ctx->buffer_pos == 0) {
        return ERROR;
    }
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    process_buffer(ctx);
    return WRITING;
}

enum state handle_writing(struct context* ctx) {
    if (ctx->buffer_pos > 0) {
        printf("Processed: %s\n", ctx->buffer);
        fflush(stdout);
    }
    return IDLE;
}

enum state handle_error(struct context* ctx) {
    ctx->error_code = 1;
    printf("Error: Invalid input detected\n");
    fflush(stdout);
    return DONE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    ctx.error_code = 0;
    
    while (1) {
        enum state next_state;
        
        switch (ctx.current_state) {
            case IDLE:
                next_state = handle_idle(&ctx);
                break;
            case READING:
                next_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                next_state = handle_processing(&ctx);
                break;
            case WRITING:
                next_state = handle_writing(&ctx);
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
        
        ctx.current_state = next_state;
    }
    
exit_loop:
    printf("Processed %d strings\n", ctx.processed_count);
    return ctx.error_code;
}
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
    int max_data;
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
    printf("Enter maximum data items to process (1-100): ");
    int max_data;
    if (scanf("%d", &max_data) != 1) {
        return ERROR;
    }
    if (max_data < 1 || max_data > 100) {
        return ERROR;
    }
    ctx->max_data = max_data;
    ctx->data_count = 0;
    return READING;
}

enum state handle_reading(struct context* ctx) {
    printf("Enter data string (or 'quit' to finish): ");
    char input[256];
    if (scanf("%255s", input) != 1) {
        return ERROR;
    }
    
    size_t len = strlen(input);
    if (!validate_input(input, len)) {
        return ERROR;
    }
    
    if (strcmp(input, "quit") == 0) {
        return DONE;
    }
    
    if (len >= sizeof(ctx->buffer)) {
        return ERROR;
    }
    
    memcpy(ctx->buffer, input, len + 1);
    ctx->buffer_len = len;
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    process_data(ctx);
    ctx->data_count++;
    return WRITING;
}

enum state handle_writing(struct context* ctx) {
    printf("Processed data: %s\n", ctx->buffer);
    if (ctx->data_count >= ctx->max_data) {
        return DONE;
    }
    return READING;
}

enum state handle_error(struct context* ctx) {
    printf("Error occurred. Resetting state machine.\n");
    ctx->current_state = IDLE;
    ctx->buffer_len = 0;
    ctx->data_count = 0;
    return IDLE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_data = 0;
    
    while (1) {
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
                printf("Processing complete. Processed %d items.\n", ctx.data_count);
                return 0;
            default:
                ctx.current_state = ERROR;
                break;
        }
        
        if (ctx.current_state == ERROR) {
            continue;
        }
    }
    
    return 0;
}
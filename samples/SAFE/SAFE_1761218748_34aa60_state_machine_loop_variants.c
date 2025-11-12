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
    int data_count;
    int max_items;
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

enum state handle_init(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    ctx->current_state = READING;
    ctx->buffer_len = 0;
    ctx->data_count = 0;
    ctx->max_items = 5;
    printf("State machine initialized. Enter up to %d data items.\n", ctx->max_items);
    return READING;
}

enum state handle_reading(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    if (ctx->data_count >= ctx->max_items) {
        return DONE;
    }
    
    printf("Enter data item %d: ", ctx->data_count + 1);
    
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
        printf("Invalid input. Please enter printable ASCII characters only.\n");
        return READING;
    }
    
    if (len >= sizeof(ctx->buffer)) {
        printf("Input too long. Maximum %zu characters allowed.\n", sizeof(ctx->buffer) - 1);
        return READING;
    }
    
    memcpy(ctx->buffer, input, len);
    ctx->buffer[len] = '\0';
    ctx->buffer_len = len;
    
    return PROCESSING;
}

enum state handle_processing(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    process_data(ctx);
    return OUTPUT;
}

enum state handle_output(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("Processed data: %s\n", ctx->buffer);
    ctx->data_count++;
    
    if (ctx->data_count >= ctx->max_items) {
        return DONE;
    }
    
    return READING;
}

enum state handle_error(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("An error occurred. Resetting state machine.\n");
    ctx->buffer_len = 0;
    ctx->data_count = 0;
    return INIT;
}

enum state handle_done(struct context* ctx) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    printf("Processing complete. Processed %d items.\n", ctx->data_count);
    return DONE;
}

int main(void) {
    struct context ctx;
    int running = 1;
    
    ctx.current_state = INIT;
    
    while (running) {
        switch (ctx.current_state) {
            case INIT:
                ctx.current_state = handle_init(&ctx);
                break;
            case READING:
                ctx.current_state = handle_reading(&ctx);
                break;
            case PROCESSING:
                ctx.current_state = handle_processing(&ctx);
                break;
            case OUTPUT:
                ctx.current_state = handle_output(&ctx);
                break;
            case ERROR:
                ctx.current_state = handle_error(&ctx);
                break;
            case DONE:
                ctx.current_state = handle_done(&ctx);
                running = 0;
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
    }
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int error_code;
};

void state_idle(struct context *ctx);
void state_reading(struct context *ctx);
void state_processing(struct context *ctx);
void state_writing(struct context *ctx);
void state_error(struct context *ctx);
void state_done(struct context *ctx);

void initialize_context(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->error_code = 0;
}

int validate_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

void state_idle(struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("Enter text to process (max 255 chars): ");
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        ctx->error_code = 1;
        return;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (!validate_input(input, sizeof(ctx->buffer))) {
        ctx->current_state = STATE_ERROR;
        ctx->error_code = 2;
        return;
    }
    
    strncpy(ctx->buffer, input, sizeof(ctx->buffer)-1);
    ctx->buffer[sizeof(ctx->buffer)-1] = '\0';
    ctx->buffer_len = strlen(ctx->buffer);
    ctx->current_state = STATE_READING;
}

void state_reading(struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->buffer_len == 0) {
        ctx->current_state = STATE_ERROR;
        ctx->error_code = 3;
        return;
    }
    
    printf("Read %zu characters: %s\n", ctx->buffer_len, ctx->buffer);
    ctx->current_state = STATE_PROCESSING;
}

void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->buffer_len == 0 || ctx->buffer_len >= sizeof(ctx->buffer)) {
        ctx->current_state = STATE_ERROR;
        ctx->error_code = 4;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: %s\n", ctx->buffer);
    ctx->current_state = STATE_WRITING;
}

void state_writing(struct context *ctx) {
    if (ctx == NULL) return;
    
    if (ctx->buffer_len == 0) {
        ctx->current_state = STATE_ERROR;
        ctx->error_code = 5;
        return;
    }
    
    printf("Final output: %s\n", ctx->buffer);
    ctx->current_state = STATE_DONE;
}

void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("Error %d occurred in state machine\n", ctx->error_code);
    ctx->current_state = STATE_DONE;
}

void state_done(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State machine completed\n");
}

void run_state_machine(struct context *ctx) {
    if (ctx == NULL) return;
    
    while (ctx->current_state != STATE_DONE) {
        switch (ctx->current_state) {
            case STATE_IDLE:
                state_idle(ctx);
                break;
            case STATE_READING:
                state_reading(ctx);
                break;
            case STATE_PROCESSING:
                state_processing(ctx);
                break;
            case STATE_WRITING:
                state_writing(ctx);
                break;
            case STATE_ERROR:
                state_error(ctx);
                break;
            case STATE_DONE:
                break;
        }
    }
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    run_state_machine(&ctx);
    return 0;
}
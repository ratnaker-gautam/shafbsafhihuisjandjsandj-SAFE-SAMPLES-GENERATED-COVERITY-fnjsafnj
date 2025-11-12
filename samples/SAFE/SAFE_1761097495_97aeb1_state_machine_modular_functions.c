//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

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
    int processed_count;
    int max_iterations;
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
    ctx->processed_count = 0;
    ctx->max_iterations = 5;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) return 0;
    }
    return 1;
}

void state_idle(struct context *ctx) {
    if (ctx == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    printf("State: IDLE - Ready to start processing\n");
    ctx->current_state = STATE_READING;
}

void state_reading(struct context *ctx) {
    if (ctx == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    printf("State: READING - Enter text to process: ");
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (!validate_input(ctx->buffer, ctx->buffer_len)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->current_state = STATE_PROCESSING;
}

void state_processing(struct context *ctx) {
    if (ctx == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    printf("State: PROCESSING - Transforming input\n");
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (islower((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)toupper((unsigned char)ctx->buffer[i]);
        } else if (isupper((unsigned char)ctx->buffer[i])) {
            ctx->buffer[i] = (char)tolower((unsigned char)ctx->buffer[i]);
        }
    }
    
    ctx->processed_count++;
    ctx->current_state = STATE_WRITING;
}

void state_writing(struct context *ctx) {
    if (ctx == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    printf("State: WRITING - Result: %s\n", ctx->buffer);
    printf("Processed %d of %d iterations\n", ctx->processed_count, ctx->max_iterations);
    
    if (ctx->processed_count >= ctx->max_iterations) {
        ctx->current_state = STATE_DONE;
    } else {
        ctx->current_state = STATE_READING;
    }
}

void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: ERROR - Invalid input or system error\n");
    ctx->current_state = STATE_DONE;
}

void state_done(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: DONE - Processing complete\n");
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
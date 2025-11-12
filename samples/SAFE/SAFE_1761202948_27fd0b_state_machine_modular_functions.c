//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) {
            return 0;
        }
    }
    return 1;
}

int read_input(struct context* ctx) {
    if (ctx == NULL) {
        return -1;
    }
    
    printf("Enter text (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return -1;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (!validate_input(ctx->buffer, ctx->buffer_len)) {
        return -1;
    }
    
    return 0;
}

int process_data(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return -1;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (islower(ctx->buffer[i])) {
            ctx->buffer[i] = toupper(ctx->buffer[i]);
        } else if (isupper(ctx->buffer[i])) {
            ctx->buffer[i] = tolower(ctx->buffer[i]);
        }
    }
    
    ctx->processed_count++;
    return 0;
}

int write_output(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return -1;
    }
    
    printf("Processed: %s\n", ctx->buffer);
    return 0;
}

void state_machine_step(struct context* ctx) {
    if (ctx == NULL) {
        return;
    }
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            if (ctx->processed_count >= ctx->max_iterations) {
                ctx->current_state = STATE_DONE;
            } else {
                ctx->current_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (read_input(ctx) == 0) {
                ctx->current_state = STATE_PROCESSING;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (process_data(ctx) == 0) {
                ctx->current_state = STATE_WRITING;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_WRITING:
            if (write_output(ctx) == 0) {
                ctx->current_state = STATE_IDLE;
            } else {
                ctx->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_ERROR:
            printf("Error occurred in processing\n");
            ctx->current_state = STATE_DONE;
            break;
            
        case STATE_DONE:
            break;
    }
}

void initialize_context(struct context* ctx) {
    if (ctx == NULL) {
        return;
    }
    
    memset(ctx, 0, sizeof(struct context));
    ctx->current_state = STATE_IDLE;
    ctx->max_iterations = 3;
}

int main(void) {
    struct context ctx;
    initialize_context(&ctx);
    
    printf("Text processing state machine (max %d iterations)\n", ctx.max_iterations);
    
    while (ctx.current_state != STATE_DONE) {
        state_machine_step(&ctx);
    }
    
    printf("Processing complete. Processed %d items.\n", ctx.processed_count);
    return 0;
}
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

void state_idle(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: IDLE\n");
    printf("Enter 'start' to begin processing: ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "start") == 0) {
            ctx->current_state = STATE_READING;
        } else {
            printf("Invalid input. Please enter 'start'.\n");
        }
    }
}

void state_reading(struct context *ctx) {
    if (ctx == NULL) return;
    printf("State: READING\n");
    printf("Enter text to process (max 255 chars): ");
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        ctx->buffer_len = strlen(ctx->buffer);
        if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
            ctx->buffer[ctx->buffer_len - 1] = 0;
            ctx->buffer_len--;
        }
        
        if (ctx->buffer_len > 0) {
            ctx->current_state = STATE_PROCESSING;
        } else {
            printf("Empty input received.\n");
            ctx->current_state = STATE_ERROR;
        }
    } else {
        printf("Error reading input.\n");
        ctx->current_state = STATE_ERROR;
    }
}

void state_processing(struct context *ctx) {
    if (ctx == NULL || ctx->buffer_len >= sizeof(ctx->buffer)) return;
    
    printf("State: PROCESSING\n");
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    ctx->current_state = STATE_WRITING;
}

void state_writing(struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("State: WRITING\n");
    printf("Processed text: %s\n", ctx->buffer);
    printf("Total processed: %d/%d\n", ctx->processed_count, ctx->max_iterations);
    
    if (ctx->processed_count >= ctx->max_iterations) {
        ctx->current_state = STATE_DONE;
    } else {
        printf("Continue? (y/n): ");
        char choice[4];
        if (fgets(choice, sizeof(choice), stdin) != NULL) {
            choice[strcspn(choice, "\n")] = 0;
            if (choice[0] == 'y' || choice[0] == 'Y') {
                ctx->current_state = STATE_READING;
            } else {
                ctx->current_state = STATE_DONE;
            }
        } else {
            ctx->current_state = STATE_ERROR;
        }
    }
}

void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("State: ERROR\n");
    printf("An error occurred. Resetting...\n");
    initialize_context(ctx);
}

void state_done(struct context *ctx) {
    if (ctx == NULL) return;
    
    printf("State: DONE\n");
    printf("Processing complete. Processed %d items.\n", ctx->processed_count);
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
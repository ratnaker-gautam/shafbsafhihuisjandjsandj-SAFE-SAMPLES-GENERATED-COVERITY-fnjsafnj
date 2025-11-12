//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int processed_count;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Enter text (or 'quit' to exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
        size_t len = strlen(ctx->buffer);
        if (len > 0 && ctx->buffer[len-1] == '\n') {
            ctx->buffer[len-1] = '\0';
        }
        
        if (strcmp(ctx->buffer, "quit") == 0) {
            exit(0);
        }
        
        ctx->buffer_pos = 0;
        ctx->current_state = READING;
    } else {
        ctx->current_state = ERROR;
    }
}

void handle_reading(struct Context* ctx) {
    if (ctx == NULL) return;
    
    size_t len = strlen(ctx->buffer);
    if (ctx->buffer_pos >= len) {
        ctx->current_state = PROCESSING;
    } else {
        ctx->buffer_pos++;
        if (ctx->buffer_pos >= len) {
            ctx->current_state = PROCESSING;
        }
    }
}

void handle_processing(struct Context* ctx) {
    if (ctx == NULL) return;
    
    size_t len = strlen(ctx->buffer);
    for (size_t i = 0; i < len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    ctx->current_state = WRITING;
}

void handle_writing(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Processed: %s\n", ctx->buffer);
    printf("Total processed: %d\n\n", ctx->processed_count);
    ctx->current_state = IDLE;
}

void handle_error(struct Context* ctx) {
    if (ctx == NULL) return;
    
    printf("Error occurred. Resetting to IDLE.\n");
    ctx->current_state = IDLE;
    ctx->buffer[0] = '\0';
    ctx->buffer_pos = 0;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    while (1) {
        if (ctx.current_state >= IDLE && ctx.current_state <= ERROR) {
            handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = ERROR;
        }
    }
    
    return 0;
}
//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_count;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    if (ctx->current_state != IDLE) return;
    
    printf("Enter data (max 255 chars) or 'quit' to exit: ");
    
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    size_t len = strlen(ctx->buffer);
    if (len > 0 && ctx->buffer[len - 1] == '\n') {
        ctx->buffer[len - 1] = '\0';
    }
    
    if (strlen(ctx->buffer) == 0) {
        return;
    }
    
    if (strcmp(ctx->buffer, "quit") == 0) {
        printf("Exiting. Processed %d data items.\n", ctx->data_count);
        exit(0);
    }
    
    ctx->buffer_pos = 0;
    ctx->current_state = READING;
}

void handle_reading(struct Context* ctx) {
    if (ctx->current_state != READING) return;
    
    size_t input_len = strlen(ctx->buffer);
    if (input_len == 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (ctx->buffer[i] < 32 || ctx->buffer[i] > 126) {
            ctx->current_state = ERROR;
            return;
        }
    }
    
    ctx->current_state = PROCESSING;
}

void handle_processing(struct Context* ctx) {
    if (ctx->current_state != PROCESSING) return;
    
    size_t len = strlen(ctx->buffer);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = ctx->buffer[i];
        ctx->buffer[i] = ctx->buffer[len - 1 - i];
        ctx->buffer[len - 1 - i] = temp;
    }
    
    ctx->current_state = WRITING;
}

void handle_writing(struct Context* ctx) {
    if (ctx->current_state != WRITING) return;
    
    printf("Processed: %s\n", ctx->buffer);
    ctx->data_count++;
    ctx->current_state = IDLE;
}

void handle_error(struct Context* ctx) {
    if (ctx->current_state != ERROR) return;
    
    printf("Error: Invalid input detected.\n");
    ctx->current_state = IDLE;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.data_count = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
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
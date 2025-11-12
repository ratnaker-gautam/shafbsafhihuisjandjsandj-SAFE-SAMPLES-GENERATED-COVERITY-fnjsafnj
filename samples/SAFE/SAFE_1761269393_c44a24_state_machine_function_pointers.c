//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_EXIT
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_len;
    int error_code;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    printf("Enter command (read, process, write, exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        ctx->error_code = 1;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->current_state = STATE_READING;
    } else if (strcmp(input, "process") == 0) {
        ctx->current_state = STATE_PROCESSING;
    } else if (strcmp(input, "write") == 0) {
        ctx->current_state = STATE_WRITING;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = STATE_EXIT;
    } else {
        printf("Invalid command\n");
    }
}

void handle_reading(struct Context* ctx) {
    printf("Enter text to read (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        ctx->error_code = 2;
        return;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    
    printf("Read %zu characters: %s\n", ctx->buffer_len, ctx->buffer);
    ctx->current_state = STATE_IDLE;
}

void handle_processing(struct Context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to process\n");
        ctx->current_state = STATE_IDLE;
        return;
    }
    
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed text: %s\n", ctx->buffer);
    ctx->current_state = STATE_IDLE;
}

void handle_writing(struct Context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to write\n");
        ctx->current_state = STATE_IDLE;
        return;
    }
    
    printf("Writing to output: %s\n", ctx->buffer);
    ctx->current_state = STATE_IDLE;
}

void handle_error(struct Context* ctx) {
    printf("Error occurred (code: %d)\n", ctx->error_code);
    ctx->current_state = STATE_EXIT;
}

void handle_exit(struct Context* ctx) {
    printf("Exiting state machine\n");
}

int main(void) {
    struct Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_len = 0;
    ctx.error_code = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_exit
    };
    
    while (ctx.current_state != STATE_EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state < (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = STATE_ERROR;
            ctx.error_code = 3;
        }
    }
    
    return 0;
}
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
    int data_value;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    printf("Enter command (read, exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->current_state = STATE_READING;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = STATE_EXIT;
    } else {
        printf("Invalid command\n");
    }
}

void handle_reading(struct Context* ctx) {
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    
    if (ctx->buffer_len == 0) {
        printf("No data entered\n");
        ctx->current_state = STATE_IDLE;
    } else {
        ctx->current_state = STATE_PROCESSING;
    }
}

void handle_processing(struct Context* ctx) {
    ctx->data_value = 0;
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->data_value > 1000000) {
            ctx->data_value = 1000000;
            break;
        }
        ctx->data_value = ctx->data_value * 31 + (int)ctx->buffer[i];
    }
    
    printf("Processed data value: %d\n", ctx->data_value);
    ctx->current_state = STATE_WRITING;
}

void handle_writing(struct Context* ctx) {
    printf("Writing result to output...\n");
    printf("Final value: %d\n", ctx->data_value);
    printf("Buffer contents: %s\n", ctx->buffer);
    ctx->current_state = STATE_IDLE;
}

void handle_error(struct Context* ctx) {
    printf("Error occurred in state machine\n");
    ctx->current_state = STATE_EXIT;
}

void handle_exit(struct Context* ctx) {
    printf("Exiting state machine\n");
}

int main(void) {
    struct Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.data_value = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_exit
    };
    
    while (ctx.current_state != STATE_EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state < 6) {
            handlers[ctx.current_state](&ctx);
        } else {
            printf("Invalid state detected\n");
            break;
        }
    }
    
    return 0;
}
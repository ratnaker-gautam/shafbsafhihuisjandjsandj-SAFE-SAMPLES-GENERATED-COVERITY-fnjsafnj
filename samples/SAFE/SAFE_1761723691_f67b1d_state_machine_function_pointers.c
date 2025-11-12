//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    EXIT
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    printf("Enter command (read, process, write, exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->current_state = READING;
    } else if (strcmp(input, "process") == 0) {
        ctx->current_state = PROCESSING;
    } else if (strcmp(input, "write") == 0) {
        ctx->current_state = WRITING;
    } else if (strcmp(input, "exit") == 0) {
        ctx->current_state = EXIT;
    } else {
        printf("Invalid command\n");
    }
}

void handle_reading(struct Context* ctx) {
    printf("Enter data to read (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    
    if (ctx->buffer_len == 0) {
        printf("No data read\n");
        ctx->current_state = IDLE;
        return;
    }
    
    printf("Read %zu characters: %s\n", ctx->buffer_len, ctx->buffer);
    ctx->current_state = IDLE;
}

void handle_processing(struct Context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to process\n");
        ctx->current_state = IDLE;
        return;
    }
    
    ctx->data_value = 0;
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            ctx->data_value = ctx->data_value * 10 + (ctx->buffer[i] - '0');
        }
    }
    
    printf("Processed data value: %d\n", ctx->data_value);
    ctx->current_state = IDLE;
}

void handle_writing(struct Context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to write\n");
        ctx->current_state = IDLE;
        return;
    }
    
    printf("Writing data: %s (value: %d)\n", ctx->buffer, ctx->data_value);
    ctx->current_state = IDLE;
}

void handle_error(struct Context* ctx) {
    printf("Error occurred in state machine\n");
    ctx->current_state = EXIT;
}

void handle_exit(struct Context* ctx) {
    printf("Exiting state machine\n");
}

int main() {
    struct Context ctx = {0};
    ctx.current_state = IDLE;
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
    
    while (ctx.current_state != EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state <= EXIT) {
            handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = ERROR;
        }
    }
    
    return 0;
}
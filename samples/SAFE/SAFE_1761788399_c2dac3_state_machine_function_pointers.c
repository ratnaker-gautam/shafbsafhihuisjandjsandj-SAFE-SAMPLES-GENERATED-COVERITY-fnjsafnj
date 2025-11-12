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
    size_t buffer_pos;
    int data_value;
};

typedef void (*StateHandler)(struct Context*);

void handle_idle(struct Context* ctx) {
    printf("Enter command (read/process/write/exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->current_state = READING;
        ctx->buffer_pos = 0;
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
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->buffer_pos = strlen(ctx->buffer);
    if (ctx->buffer_pos > 0 && ctx->buffer[ctx->buffer_pos - 1] == '\n') {
        ctx->buffer[--ctx->buffer_pos] = '\0';
    }
    
    printf("Read %zu characters\n", ctx->buffer_pos);
    ctx->current_state = IDLE;
}

void handle_processing(struct Context* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to process\n");
        ctx->current_state = IDLE;
        return;
    }
    
    int sum = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        sum += (int)ctx->buffer[i];
    }
    
    if (ctx->buffer_pos > 0) {
        ctx->data_value = sum / (int)ctx->buffer_pos;
    } else {
        ctx->data_value = 0;
    }
    
    printf("Processed data: average ASCII value = %d\n", ctx->data_value);
    ctx->current_state = IDLE;
}

void handle_writing(struct Context* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to write\n");
    } else {
        printf("Data: %s\n", ctx->buffer);
        printf("Processed value: %d\n", ctx->data_value);
    }
    ctx->current_state = IDLE;
}

void handle_error(struct Context* ctx) {
    printf("Error occurred\n");
    ctx->current_state = EXIT;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_pos = 0;
    ctx.data_value = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        NULL
    };
    
    while (ctx.current_state != EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state < EXIT) {
            handlers[ctx.current_state](&ctx);
        } else {
            ctx.current_state = ERROR;
        }
    }
    
    printf("Goodbye\n");
    return 0;
}
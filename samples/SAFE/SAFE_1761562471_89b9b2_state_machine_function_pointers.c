//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_EXIT
} State;

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    printf("Enter command (read/process/write/exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        return STATE_READING;
    } else if (strcmp(input, "process") == 0) {
        return STATE_PROCESSING;
    } else if (strcmp(input, "write") == 0) {
        return STATE_WRITING;
    } else if (strcmp(input, "exit") == 0) {
        return STATE_EXIT;
    } else {
        printf("Invalid command\n");
        return STATE_IDLE;
    }
}

State handle_reading(Context* ctx) {
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    
    if (ctx->buffer_len == 0) {
        printf("No data entered\n");
        return STATE_IDLE;
    }
    
    printf("Read %zu characters\n", ctx->buffer_len);
    return STATE_IDLE;
}

State handle_processing(Context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to process\n");
        return STATE_IDLE;
    }
    
    ctx->data_value = 0;
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            ctx->data_value += (ctx->buffer[i] - '0');
        }
    }
    
    printf("Processed data - sum of digits: %d\n", ctx->data_value);
    return STATE_IDLE;
}

State handle_writing(Context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to write\n");
        return STATE_IDLE;
    }
    
    printf("Data: %s\n", ctx->buffer);
    if (ctx->data_value != 0) {
        printf("Processed value: %d\n", ctx->data_value);
    }
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    printf("An error occurred\n");
    return STATE_EXIT;
}

State handle_exit(Context* ctx) {
    printf("Goodbye\n");
    return STATE_EXIT;
}

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.data_value = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        handle_exit
    };
    
    while (ctx.current_state != STATE_EXIT) {
        if (ctx.current_state < 0 || ctx.current_state >= (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            ctx.current_state = STATE_ERROR;
        }
        
        State next_state = handlers[ctx.current_state](&ctx);
        ctx.current_state = next_state;
    }
    
    return 0;
}
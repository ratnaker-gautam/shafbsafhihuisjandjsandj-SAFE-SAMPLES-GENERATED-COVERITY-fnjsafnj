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
    printf("Enter data to read (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[--ctx->buffer_len] = '\0';
    }
    
    if (ctx->buffer_len == 0) {
        printf("No data read\n");
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
            ctx->data_value = ctx->data_value * 10 + (ctx->buffer[i] - '0');
            if (ctx->data_value < 0) {
                printf("Integer overflow detected\n");
                return STATE_ERROR;
            }
        }
    }
    
    printf("Processed value: %d\n", ctx->data_value);
    return STATE_IDLE;
}

State handle_writing(Context* ctx) {
    if (ctx->buffer_len == 0) {
        printf("No data to write\n");
        return STATE_IDLE;
    }
    
    printf("Writing data: %s\n", ctx->buffer);
    printf("Processed value: %d\n", ctx->data_value);
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    printf("Error occurred, resetting to idle\n");
    ctx->buffer_len = 0;
    ctx->data_value = 0;
    return STATE_IDLE;
}

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.data_value = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error,
        NULL
    };
    
    while (ctx.current_state != STATE_EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state < STATE_EXIT) {
            StateHandler handler = handlers[ctx.current_state];
            if (handler != NULL) {
                ctx.current_state = handler(&ctx);
            } else {
                break;
            }
        } else {
            break;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}
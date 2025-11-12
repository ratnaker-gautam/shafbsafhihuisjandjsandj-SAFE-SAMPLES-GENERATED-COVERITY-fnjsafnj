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
} state_t;

typedef struct {
    state_t current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_value;
} context_t;

typedef state_t (*state_handler_t)(context_t*);

state_t handle_idle(context_t* ctx) {
    printf("Enter command (read/process/write/exit): ");
    char input[32];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        ctx->buffer_pos = 0;
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

state_t handle_reading(context_t* ctx) {
    printf("Enter data (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer_pos = strlen(ctx->buffer);
    if (ctx->buffer_pos > 0 && ctx->buffer[ctx->buffer_pos - 1] == '\n') {
        ctx->buffer[--ctx->buffer_pos] = '\0';
    }
    
    if (ctx->buffer_pos == 0) {
        printf("No data entered\n");
        return STATE_IDLE;
    }
    
    printf("Read %zu characters\n", ctx->buffer_pos);
    return STATE_IDLE;
}

state_t handle_processing(context_t* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to process\n");
        return STATE_IDLE;
    }
    
    ctx->data_value = 0;
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
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

state_t handle_writing(context_t* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to write\n");
        return STATE_IDLE;
    }
    
    printf("Data: %s\n", ctx->buffer);
    if (ctx->data_value != 0) {
        printf("Processed value: %d\n", ctx->data_value);
    }
    return STATE_IDLE;
}

state_t handle_error(context_t* ctx) {
    printf("Error occurred. Resetting state machine.\n");
    ctx->buffer_pos = 0;
    ctx->data_value = 0;
    return STATE_IDLE;
}

int main(void) {
    context_t ctx = {STATE_IDLE, "", 0, 0};
    state_handler_t handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    while (ctx.current_state != STATE_EXIT) {
        if (ctx.current_state >= 0 && ctx.current_state < 5) {
            ctx.current_state = handlers[ctx.current_state](&ctx);
        } else {
            printf("Invalid state\n");
            break;
        }
    }
    
    printf("State machine terminated\n");
    return 0;
}
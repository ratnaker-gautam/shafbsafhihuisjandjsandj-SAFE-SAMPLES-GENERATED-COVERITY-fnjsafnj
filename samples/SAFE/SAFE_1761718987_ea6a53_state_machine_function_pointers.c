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
    size_t buffer_pos;
    int error_code;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    printf("Enter command (read/process/write/exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->error_code = 1;
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "read") == 0) {
        return STATE_READING;
    } else if (strcmp(ctx->buffer, "process") == 0) {
        return STATE_PROCESSING;
    } else if (strcmp(ctx->buffer, "write") == 0) {
        return STATE_WRITING;
    } else if (strcmp(ctx->buffer, "exit") == 0) {
        return STATE_EXIT;
    } else {
        printf("Invalid command\n");
        return STATE_IDLE;
    }
}

State handle_reading(Context* ctx) {
    printf("Enter data to read (max 255 chars): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
        ctx->error_code = 2;
        return STATE_ERROR;
    }
    
    ctx->buffer_pos = strlen(ctx->buffer);
    if (ctx->buffer_pos > 0 && ctx->buffer[ctx->buffer_pos - 1] == '\n') {
        ctx->buffer[ctx->buffer_pos - 1] = '\0';
        ctx->buffer_pos--;
    }
    
    printf("Read %zu characters: %s\n", ctx->buffer_pos, ctx->buffer);
    return STATE_IDLE;
}

State handle_processing(Context* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to process\n");
        return STATE_IDLE;
    }
    
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    printf("Processed data: %s\n", ctx->buffer);
    return STATE_IDLE;
}

State handle_writing(Context* ctx) {
    if (ctx->buffer_pos == 0) {
        printf("No data to write\n");
        return STATE_IDLE;
    }
    
    printf("Writing data: %s\n", ctx->buffer);
    printf("Data written successfully\n");
    ctx->buffer[0] = '\0';
    ctx->buffer_pos = 0;
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    printf("Error occurred (code: %d)\n", ctx->error_code);
    ctx->error_code = 0;
    return STATE_IDLE;
}

State handle_exit(Context* ctx) {
    printf("Exiting state machine\n");
    return STATE_EXIT;
}

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_pos = 0;
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
        if (ctx.current_state < 0 || ctx.current_state >= (int)(sizeof(handlers)/sizeof(handlers[0]))) {
            printf("Invalid state\n");
            break;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            printf("No handler for state\n");
            break;
        }
        
        State next_state = handler(&ctx);
        ctx.current_state = next_state;
    }
    
    return 0;
}
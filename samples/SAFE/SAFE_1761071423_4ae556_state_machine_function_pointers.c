//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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
    int buffer_pos;
    int processed_count;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    printf("Enter command (read/process/write/exit): ");
    if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) == NULL) {
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
        return STATE_ERROR;
    }
    
    ctx->buffer_pos = strcspn(ctx->buffer, "\n");
    ctx->buffer[ctx->buffer_pos] = '\0';
    
    if (ctx->buffer_pos > 0) {
        printf("Read %d characters\n", ctx->buffer_pos);
        return STATE_IDLE;
    } else {
        printf("No data read\n");
        return STATE_IDLE;
    }
}

State handle_processing(Context* ctx) {
    if (ctx->buffer_pos <= 0) {
        printf("No data to process\n");
        return STATE_IDLE;
    }
    
    for (int i = 0; i < ctx->buffer_pos; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    
    ctx->processed_count++;
    printf("Processed data (converted to uppercase), total processed: %d\n", ctx->processed_count);
    return STATE_IDLE;
}

State handle_writing(Context* ctx) {
    if (ctx->buffer_pos <= 0) {
        printf("No data to write\n");
        return STATE_IDLE;
    }
    
    printf("Writing data: %s\n", ctx->buffer);
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    printf("Error occurred in state machine\n");
    return STATE_EXIT;
}

State handle_exit(Context* ctx) {
    printf("Exiting state machine. Total operations processed: %d\n", ctx->processed_count);
    return STATE_EXIT;
}

int main(void) {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_pos = 0;
    ctx.processed_count = 0;
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
            printf("Invalid state detected\n");
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
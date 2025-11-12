//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_EXIT
} State;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    int buffer_len;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    printf("Enter command (read/process/write/exit): ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_EXIT;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (strcmp(ctx->buffer, "read") == 0) {
        return STATE_READING;
    } else if (strcmp(ctx->buffer, "process") == 0) {
        return STATE_PROCESSING;
    } else if (strcmp(ctx->buffer, "write") == 0) {
        return STATE_WRITING;
    } else if (strcmp(ctx->buffer, "exit") == 0) {
        return STATE_EXIT;
    } else {
        printf("Unknown command: %s\n", ctx->buffer);
        return STATE_IDLE;
    }
}

State handle_reading(Context* ctx) {
    printf("Reading data... Enter text: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_EXIT;
    }
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (ctx->buffer_len > 0) {
        printf("Read %d characters: %s\n", ctx->buffer_len, ctx->buffer);
    } else {
        printf("No data read\n");
    }
    
    return STATE_IDLE;
}

State handle_processing(Context* ctx) {
    if (ctx->buffer_len <= 0) {
        printf("No data to process\n");
        return STATE_IDLE;
    }
    
    printf("Processing data...\n");
    for (int i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    printf("Processed: %s\n", ctx->buffer);
    return STATE_IDLE;
}

State handle_writing(Context* ctx) {
    if (ctx->buffer_len <= 0) {
        printf("No data to write\n");
        return STATE_IDLE;
    }
    
    printf("Writing data: %s\n", ctx->buffer);
    printf("Data written successfully\n");
    return STATE_IDLE;
}

State handle_exit(Context* ctx) {
    printf("Exiting state machine\n");
    return STATE_EXIT;
}

int main() {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    memset(ctx.buffer, 0, MAX_INPUT_LEN);
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_exit
    };
    
    printf("State Machine Demo Started\n");
    
    while (ctx.current_state != STATE_EXIT) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            printf("Invalid state detected, resetting to IDLE\n");
            ctx.current_state = STATE_IDLE;
            continue;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            printf("No handler for state %d, exiting\n", ctx.current_state);
            break;
        }
        
        State next_state = handler(&ctx);
        ctx.current_state = next_state;
    }
    
    printf("State Machine Demo Ended\n");
    return 0;
}
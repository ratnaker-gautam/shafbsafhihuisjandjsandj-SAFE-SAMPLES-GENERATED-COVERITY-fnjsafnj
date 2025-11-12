//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    int buffer_len;
    int processed_count;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("System idle. Enter 'start' to begin: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "start") == 0) {
        printf("System starting...\n");
        return STATE_READY;
    }
    
    printf("Invalid command. Please enter 'start'.\n");
    return STATE_IDLE;
}

State handle_ready(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("System ready. Enter data to process (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer_len = strcspn(ctx->buffer, "\n");
    ctx->buffer[ctx->buffer_len] = '\0';
    
    if (ctx->buffer_len == 0) {
        printf("Empty input. Please enter valid data.\n");
        return STATE_READY;
    }
    
    printf("Processing data: %s\n", ctx->buffer);
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    int sum = 0;
    for (int i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            sum += ctx->buffer[i] - '0';
        }
    }
    
    printf("Processing complete. Sum of digits: %d\n", sum);
    ctx->processed_count++;
    
    printf("Processed %d items. Continue? (y/n): ", ctx->processed_count);
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "y") == 0 || strcmp(ctx->buffer, "yes") == 0) {
        return STATE_READY;
    } else if (strcmp(ctx->buffer, "n") == 0 || strcmp(ctx->buffer, "no") == 0) {
        return STATE_COMPLETE;
    }
    
    printf("Invalid response. Please enter 'y' or 'n'.\n");
    return STATE_PROCESSING;
}

State handle_complete(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Processing complete. Total items processed: %d\n", ctx->processed_count);
    printf("Goodbye!\n");
    return STATE_COMPLETE;
}

State handle_error(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred. Resetting system.\n");
    ctx->processed_count = 0;
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
    return STATE_IDLE;
}

int main() {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    memset(ctx.buffer, 0, MAX_INPUT_LEN);
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    while (1) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            printf("Invalid state detected. Exiting.\n");
            return 1;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        State next_state = handler(&ctx);
        
        if (next_state < 0 || next_state >= MAX_STATES) {
            printf("Invalid next state. Exiting.\n");
            return 1;
        }
        
        if (ctx.current_state == STATE_COMPLETE && next_state == STATE_COMPLETE) {
            break;
        }
        
        ctx.current_state = next_state;
    }
    
    return 0;
}
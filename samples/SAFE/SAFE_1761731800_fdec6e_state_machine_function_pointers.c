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
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("System idle. Enter 'start' to begin: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) return STATE_ERROR;
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (strcmp(ctx->buffer, "start") == 0) {
        return STATE_READY;
    }
    
    printf("Invalid command. Please enter 'start'.\n");
    return STATE_IDLE;
}

State handle_ready(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("System ready. Enter data to process: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) return STATE_ERROR;
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (ctx->buffer_len == 0) {
        printf("No data entered.\n");
        return STATE_READY;
    }
    
    if (ctx->buffer_len >= MAX_INPUT_LEN - 1) {
        printf("Input too long.\n");
        return STATE_ERROR;
    }
    
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Processing data: %s\n", ctx->buffer);
    
    int sum = 0;
    for (int i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            sum += ctx->buffer[i] - '0';
        }
    }
    
    printf("Sum of digits in input: %d\n", sum);
    
    return STATE_COMPLETE;
}

State handle_complete(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Processing complete. Enter 'restart' to continue or 'quit' to exit: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) return STATE_ERROR;
    
    ctx->buffer_len = strlen(ctx->buffer);
    if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
        ctx->buffer[ctx->buffer_len - 1] = '\0';
        ctx->buffer_len--;
    }
    
    if (strcmp(ctx->buffer, "restart") == 0) {
        return STATE_IDLE;
    } else if (strcmp(ctx->buffer, "quit") == 0) {
        return STATE_ERROR;
    }
    
    printf("Invalid command. Please enter 'restart' or 'quit'.\n");
    return STATE_COMPLETE;
}

State handle_error(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    printf("Error state reached. Exiting.\n");
    return STATE_ERROR;
}

int main() {
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.buffer_len = 0;
    memset(ctx.buffer, 0, MAX_INPUT_LEN);
    
    while (ctx.current_state != STATE_ERROR) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            break;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            break;
        }
        
        State next_state = handler(&ctx);
        ctx.current_state = next_state;
    }
    
    return 0;
}
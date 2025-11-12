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
    int data;
    char buffer[MAX_INPUT_LEN];
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("IDLE: Enter 'start' to begin: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "start") == 0) {
        printf("Transitioning to READY state\n");
        return STATE_READY;
    } else {
        printf("Invalid command. Staying in IDLE.\n");
        return STATE_IDLE;
    }
}

State handle_ready(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("READY: Enter a number (0-100): ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    char* endptr;
    long value = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        printf("Invalid number format.\n");
        return STATE_READY;
    }
    
    if (value < 0 || value > 100) {
        printf("Number out of range (0-100).\n");
        return STATE_READY;
    }
    
    ctx->data = (int)value;
    printf("Number %d accepted. Transitioning to PROCESSING.\n", ctx->data);
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("PROCESSING: Calculating square of %d...\n", ctx->data);
    
    if (ctx->data > 46340) {
        printf("Number too large for safe squaring.\n");
        return STATE_ERROR;
    }
    
    int result = ctx->data * ctx->data;
    printf("Result: %d^2 = %d\n", ctx->data, result);
    
    printf("Processing complete. Transitioning to COMPLETE.\n");
    return STATE_COMPLETE;
}

State handle_complete(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("COMPLETE: Enter 'reset' to start over or 'quit' to exit: ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "reset") == 0) {
        printf("Resetting to IDLE state.\n");
        return STATE_IDLE;
    } else if (strcmp(ctx->buffer, "quit") == 0) {
        printf("Exiting state machine.\n");
        return STATE_ERROR;
    } else {
        printf("Invalid command. Staying in COMPLETE.\n");
        return STATE_COMPLETE;
    }
}

State handle_error(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("ERROR: An error occurred. Exiting.\n");
    return STATE_ERROR;
}

int main() {
    Context ctx;
    memset(&ctx, 0, sizeof(Context));
    ctx.current_state = STATE_IDLE;
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    while (ctx.current_state != STATE_ERROR) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            printf("Invalid state detected. Exiting.\n");
            break;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            printf("No handler for state %d. Exiting.\n", ctx.current_state);
            break;
        }
        
        State next_state = handler(&ctx);
        if (next_state < 0 || next_state >= MAX_STATES) {
            printf("Invalid next state %d. Exiting.\n", next_state);
            break;
        }
        
        ctx.current_state = next_state;
    }
    
    return 0;
}
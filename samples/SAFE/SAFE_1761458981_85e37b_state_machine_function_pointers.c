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
    int data_length;
} Context;

typedef State (*StateHandler)(Context*);

State handle_idle(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter command (start/quit): ");
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->buffer[strcspn(ctx->buffer, "\n")] = '\0';
    
    if (strcmp(ctx->buffer, "start") == 0) {
        printf("Transitioning to READY state\n");
        return STATE_READY;
    } else if (strcmp(ctx->buffer, "quit") == 0) {
        printf("Exiting state machine\n");
        return STATE_COMPLETE;
    } else {
        printf("Invalid command. Please enter 'start' or 'quit'\n");
        return STATE_IDLE;
    }
}

State handle_ready(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Enter data to process (max %d chars): ", MAX_INPUT_LEN - 1);
    if (fgets(ctx->buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return STATE_ERROR;
    }
    
    ctx->data_length = strcspn(ctx->buffer, "\n");
    ctx->buffer[ctx->data_length] = '\0';
    
    if (ctx->data_length == 0) {
        printf("No data entered. Returning to IDLE state\n");
        return STATE_IDLE;
    }
    
    if (ctx->data_length >= MAX_INPUT_LEN - 1) {
        printf("Input too long. Truncated to %d characters\n", MAX_INPUT_LEN - 1);
    }
    
    printf("Data received. Transitioning to PROCESSING state\n");
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL || ctx->data_length <= 0) return STATE_ERROR;
    
    printf("Processing data: '");
    for (int i = 0; i < ctx->data_length; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            putchar(ctx->buffer[i] - 32);
        } else if (ctx->buffer[i] >= 'A' && ctx->buffer[i] <= 'Z') {
            putchar(ctx->buffer[i] + 32);
        } else {
            putchar(ctx->buffer[i]);
        }
    }
    printf("'\n");
    
    printf("Processing complete. Transitioning to COMPLETE state\n");
    return STATE_COMPLETE;
}

State handle_complete(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Operation completed successfully. Returning to IDLE state\n");
    ctx->data_length = 0;
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
    return STATE_IDLE;
}

State handle_error(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Error occurred. Resetting state machine\n");
    ctx->data_length = 0;
    memset(ctx->buffer, 0, MAX_INPUT_LEN);
    return STATE_IDLE;
}

int main() {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.data_length = 0;
    memset(ctx.buffer, 0, MAX_INPUT_LEN);
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    printf("State Machine Demo Started\n");
    printf("Available states: IDLE, READY, PROCESSING, COMPLETE, ERROR\n\n");
    
    while (ctx.current_state != STATE_COMPLETE) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            printf("Invalid state detected. Resetting to IDLE\n");
            ctx.current_state = STATE_IDLE;
            continue;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            printf("No handler for state %d. Resetting to IDLE\n", ctx.current_state);
            ctx.current_state = STATE_IDLE;
            continue;
        }
        
        State next_state = handler(&ctx);
        
        if (next_state < 0 || next_state >= MAX_STATES) {
            printf("Invalid next state %d. Transitioning to ERROR state\n", next_state);
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.current_state = next_state;
        }
    }
    
    printf("State Machine Demo Ended\n");
    return 0;
}
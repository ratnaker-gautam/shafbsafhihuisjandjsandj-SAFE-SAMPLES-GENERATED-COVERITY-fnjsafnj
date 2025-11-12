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
        printf("Invalid command\n");
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
        printf("No data entered\n");
        return STATE_READY;
    }
    
    printf("Data received: %s\n", ctx->buffer);
    return STATE_PROCESSING;
}

State handle_processing(Context* ctx) {
    if (ctx == NULL || ctx->data_length <= 0) return STATE_ERROR;
    
    printf("Processing data...\n");
    
    int char_count = 0;
    int digit_count = 0;
    
    for (int i = 0; i < ctx->data_length; i++) {
        if (ctx->buffer[i] >= '0' && ctx->buffer[i] <= '9') {
            digit_count++;
        } else if ((ctx->buffer[i] >= 'A' && ctx->buffer[i] <= 'Z') || 
                  (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z')) {
            char_count++;
        }
    }
    
    printf("Analysis complete: %d letters, %d digits\n", char_count, digit_count);
    return STATE_COMPLETE;
}

State handle_complete(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("Process completed successfully\n");
    return STATE_COMPLETE;
}

State handle_error(Context* ctx) {
    if (ctx == NULL) return STATE_ERROR;
    
    printf("An error occurred\n");
    return STATE_ERROR;
}

int main() {
    Context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.data_length = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    while (ctx.current_state != STATE_COMPLETE && ctx.current_state != STATE_ERROR) {
        if (ctx.current_state < 0 || ctx.current_state >= MAX_STATES) {
            printf("Invalid state detected\n");
            break;
        }
        
        StateHandler handler = handlers[ctx.current_state];
        if (handler == NULL) {
            printf("No handler for state %d\n", ctx.current_state);
            break;
        }
        
        State next_state = handler(&ctx);
        if (next_state < 0 || next_state >= MAX_STATES) {
            printf("Invalid next state: %d\n", next_state);
            break;
        }
        
        ctx.current_state = next_state;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        printf("Terminating due to error\n");
        return 1;
    }
    
    printf("State machine terminated normally\n");
    return 0;
}
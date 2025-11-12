//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int counter;
} Context;

typedef void (*StateHandler)(Context*);

void handle_idle(Context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->counter < 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter > 0) {
        ctx->current_state = STATE_READY;
        ctx->data = 0;
    }
}

void handle_ready(Context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->counter < 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter > 100) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter > 0) {
        ctx->current_state = STATE_PROCESSING;
        ctx->data = ctx->counter * 2;
    } else {
        ctx->current_state = STATE_IDLE;
    }
}

void handle_processing(Context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data < 0 || ctx->data > 1000) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->data > 100) {
        ctx->current_state = STATE_COMPLETE;
    } else {
        ctx->current_state = STATE_READY;
        ctx->counter = ctx->data / 2;
    }
}

void handle_complete(Context* ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->counter = 0;
    ctx->data = 0;
}

void handle_error(Context* ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    ctx->counter = 0;
    ctx->data = 0;
}

StateHandler get_handler(State state) {
    static StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        return handlers[state];
    }
    
    return handle_error;
}

const char* state_name(State state) {
    static const char* names[] = {
        "IDLE",
        "READY",
        "PROCESSING",
        "COMPLETE",
        "ERROR"
    };
    
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        return names[state];
    }
    
    return "UNKNOWN";
}

int main(void) {
    Context ctx = {STATE_IDLE, 0, 0};
    int input;
    char buffer[32];
    
    printf("State Machine Demo\n");
    printf("Enter positive numbers to advance states, negative to reset, 0 to exit\n");
    
    while (1) {
        printf("Current state: %s, Data: %d, Counter: %d\n", 
               state_name(ctx.current_state), ctx.data, ctx.counter);
        printf("Enter value: ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(buffer, "%d", &input) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input == 0) {
            break;
        }
        
        if (input < 0) {
            ctx.current_state = STATE_IDLE;
            ctx.data = 0;
            ctx.counter = 0;
            continue;
        }
        
        if (input > 10000) {
            printf("Input too large. Using 10000.\n");
            input = 10000;
        }
        
        ctx.counter = input;
        
        StateHandler handler = get_handler(ctx.current_state);
        if (handler != NULL) {
            handler(&ctx);
        }
        
        if (ctx.current_state == STATE_COMPLETE) {
            printf("Processing complete! Result: %d\n", ctx.data);
        } else if (ctx.current_state == STATE_ERROR) {
            printf("Error occurred! Resetting...\n");
        }
    }
    
    printf("Exiting state machine.\n");
    return 0;
}
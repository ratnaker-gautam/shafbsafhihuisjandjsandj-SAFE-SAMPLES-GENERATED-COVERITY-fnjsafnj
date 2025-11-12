//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
};

struct context {
    enum state current_state;
    int data;
    int counter;
};

typedef void (*state_handler)(struct context*);

void state_idle(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->counter < 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter > 100) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter == 0) {
        ctx->current_state = STATE_READY;
        ctx->counter = 1;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

void state_ready(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data < 0 || ctx->data > 1000) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->data > 0 && ctx->data <= 1000) {
        ctx->current_state = STATE_PROCESSING;
    } else {
        ctx->current_state = STATE_ERROR;
    }
}

void state_processing(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data < 1 || ctx->data > 1000) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    int result;
    if (__builtin_mul_overflow(ctx->data, 2, &result)) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    ctx->data = result;
    
    if (ctx->data > 500) {
        ctx->current_state = STATE_COMPLETE;
    } else {
        ctx->current_state = STATE_READY;
    }
}

void state_complete(struct context* ctx) {
    if (ctx == NULL) return;
    ctx->counter = 0;
}

void state_error(struct context* ctx) {
    if (ctx == NULL) return;
    ctx->data = -1;
    ctx->counter = -1;
}

int main(void) {
    static state_handler handlers[STATE_COUNT] = {
        state_idle,
        state_ready,
        state_processing,
        state_complete,
        state_error
    };
    
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.data = 0;
    ctx.counter = 0;
    
    char input[32];
    int iterations = 0;
    const int max_iterations = 50;
    
    printf("Enter initial data value (1-100): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    char* endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (value < 1 || value > 100) {
        printf("Value out of range\n");
        return EXIT_FAILURE;
    }
    
    ctx.data = (int)value;
    
    while (ctx.current_state != STATE_COMPLETE && 
           ctx.current_state != STATE_ERROR &&
           iterations < max_iterations) {
        
        if (ctx.current_state < 0 || ctx.current_state >= STATE_COUNT) {
            printf("Invalid state\n");
            return EXIT_FAILURE;
        }
        
        handlers[ctx.current_state](&ctx);
        iterations++;
        
        printf("State: %d, Data: %d, Counter: %d\n", 
               ctx.current_state, ctx.data, ctx.counter);
    }
    
    if (ctx.current_state == STATE_COMPLETE) {
        printf("Processing completed successfully\n");
        printf("Final data value: %d\n", ctx.data);
    } else if (ctx.current_state == STATE_ERROR) {
        printf("Processing failed with error\n");
        return EXIT_FAILURE;
    } else {
        printf("Processing terminated (max iterations)\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
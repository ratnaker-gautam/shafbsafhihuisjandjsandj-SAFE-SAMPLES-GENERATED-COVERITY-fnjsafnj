//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
};

struct context {
    enum state current_state;
    int data;
    int counter;
};

typedef void (*state_handler)(struct context*);

void handle_idle(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data < 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->data > 0) {
        ctx->current_state = STATE_READY;
        ctx->counter = 0;
    }
}

void handle_ready(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data <= 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter >= 3) {
        ctx->current_state = STATE_PROCESSING;
        ctx->counter = 0;
    } else {
        ctx->counter++;
    }
}

void handle_processing(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data <= 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->counter >= 5) {
        ctx->current_state = STATE_DONE;
        ctx->data = ctx->data * 2;
    } else {
        ctx->counter++;
    }
}

void handle_done(struct context* ctx) {
    if (ctx == NULL) return;
    
    printf("Processing complete. Result: %d\n", ctx->data);
    ctx->current_state = STATE_IDLE;
    ctx->data = 0;
    ctx->counter = 0;
}

void handle_error(struct context* ctx) {
    if (ctx == NULL) return;
    
    printf("Error state reached. Resetting.\n");
    ctx->current_state = STATE_IDLE;
    ctx->data = 0;
    ctx->counter = 0;
}

state_handler get_handler(enum state s) {
    switch (s) {
        case STATE_IDLE: return handle_idle;
        case STATE_READY: return handle_ready;
        case STATE_PROCESSING: return handle_processing;
        case STATE_DONE: return handle_done;
        case STATE_ERROR: return handle_error;
        default: return NULL;
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.data = 0;
    ctx.counter = 0;
    
    char input[32];
    int value;
    
    printf("State Machine Demo\n");
    printf("Enter positive integers to process, 0 to reset, negative to quit:\n");
    
    while (1) {
        printf("Current state: ");
        switch (ctx.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" | Data: %d | Counter: %d\n", ctx.data, ctx.counter);
        
        printf("Enter value: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strlen(input) == 0 || input[0] == '\n') {
            continue;
        }
        
        char* endptr;
        value = (int)strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < 0) {
            printf("Exiting...\n");
            break;
        }
        
        if (value == 0) {
            ctx.current_state = STATE_IDLE;
            ctx.data = 0;
            ctx.counter = 0;
            continue;
        }
        
        ctx.data = value;
        
        state_handler handler = get_handler(ctx.current_state);
        if (handler != NULL) {
            handler(&ctx);
        }
        
        for (int i = 0; i < 10 && ctx.current_state != STATE_IDLE && ctx.current_state != STATE_ERROR; i++) {
            handler = get_handler(ctx.current_state);
            if (handler != NULL) {
                handler(&ctx);
            } else {
                break;
            }
        }
    }
    
    return 0;
}
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
    int data_value;
    int step_count;
};

typedef void (*state_handler)(struct context*);

void handle_idle(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data_value < 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->data_value > 0) {
        ctx->current_state = STATE_READY;
        ctx->step_count = 0;
    }
}

void handle_ready(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data_value <= 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->step_count >= 3) {
        ctx->current_state = STATE_PROCESSING;
        ctx->step_count = 0;
    } else {
        ctx->step_count++;
    }
}

void handle_processing(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data_value <= 0) {
        ctx->current_state = STATE_ERROR;
        return;
    }
    
    if (ctx->step_count >= 5) {
        ctx->current_state = STATE_DONE;
        ctx->data_value *= 2;
    } else {
        ctx->data_value += 10;
        ctx->step_count++;
    }
}

void handle_done(struct context* ctx) {
    if (ctx == NULL) return;
    printf("Processing complete. Final value: %d\n", ctx->data_value);
    ctx->current_state = STATE_IDLE;
    ctx->data_value = 0;
    ctx->step_count = 0;
}

void handle_error(struct context* ctx) {
    if (ctx == NULL) return;
    printf("Error state reached. Resetting.\n");
    ctx->current_state = STATE_IDLE;
    ctx->data_value = 0;
    ctx->step_count = 0;
}

state_handler get_handler(enum state s) {
    switch (s) {
        case STATE_IDLE: return handle_idle;
        case STATE_READY: return handle_ready;
        case STATE_PROCESSING: return handle_processing;
        case STATE_DONE: return handle_done;
        case STATE_ERROR: return handle_error;
        default: return handle_error;
    }
}

int main() {
    struct context ctx = {STATE_IDLE, 0, 0};
    char input_buffer[32];
    int input_value;
    
    printf("State Machine Demo\n");
    printf("Enter positive integers to process (negative to reset, 0 to exit):\n");
    
    while (1) {
        printf("Current state: %d, Data: %d, Steps: %d\n", 
               ctx.current_state, ctx.data_value, ctx.step_count);
        printf("Enter value: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (sscanf(input_buffer, "%d", &input_value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (input_value == 0) {
            break;
        }
        
        if (input_value < 0) {
            ctx.current_state = STATE_ERROR;
        } else {
            ctx.data_value = input_value;
        }
        
        state_handler handler = get_handler(ctx.current_state);
        if (handler != NULL) {
            handler(&ctx);
        }
        
        if (ctx.current_state == STATE_DONE) {
            handler = get_handler(ctx.current_state);
            if (handler != NULL) {
                handler(&ctx);
            }
        }
    }
    
    printf("Exiting state machine.\n");
    return 0;
}
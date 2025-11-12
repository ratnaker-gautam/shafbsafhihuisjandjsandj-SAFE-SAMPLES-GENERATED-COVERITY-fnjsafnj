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
    } else if (ctx->data_value > 0) {
        ctx->current_state = STATE_READY;
        ctx->step_count = 0;
    }
}

void handle_ready(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data_value <= 0) {
        ctx->current_state = STATE_ERROR;
    } else if (ctx->step_count >= 3) {
        ctx->current_state = STATE_PROCESSING;
    } else {
        ctx->step_count++;
    }
}

void handle_processing(struct context* ctx) {
    if (ctx == NULL) return;
    
    if (ctx->data_value <= 0) {
        ctx->current_state = STATE_ERROR;
    } else {
        ctx->data_value -= 10;
        if (ctx->data_value <= 0) {
            ctx->current_state = STATE_DONE;
        }
    }
}

void handle_done(struct context* ctx) {
    if (ctx == NULL) return;
    ctx->data_value = 0;
}

void handle_error(struct context* ctx) {
    if (ctx == NULL) return;
    ctx->data_value = -1;
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

void process_state(struct context* ctx) {
    if (ctx == NULL) return;
    
    state_handler handler = get_handler(ctx->current_state);
    if (handler != NULL) {
        handler(ctx);
    }
}

void print_state_info(struct context* ctx) {
    if (ctx == NULL) return;
    
    const char* state_names[] = {
        "IDLE", "READY", "PROCESSING", "DONE", "ERROR"
    };
    
    printf("State: %s, Data: %d, Steps: %d\n", 
           state_names[ctx->current_state], 
           ctx->data_value, 
           ctx->step_count);
}

int main() {
    struct context ctx;
    ctx.current_state = STATE_IDLE;
    ctx.step_count = 0;
    
    printf("Enter initial data value: ");
    if (scanf("%d", &ctx.data_value) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    printf("Starting state machine...\n");
    
    int max_iterations = 50;
    int iteration = 0;
    
    while (ctx.current_state != STATE_DONE && 
           ctx.current_state != STATE_ERROR && 
           iteration < max_iterations) {
        
        print_state_info(&ctx);
        process_state(&ctx);
        iteration++;
        
        if (iteration >= max_iterations) {
            printf("Maximum iterations reached\n");
            break;
        }
    }
    
    print_state_info(&ctx);
    
    if (ctx.current_state == STATE_DONE) {
        printf("Processing completed successfully\n");
    } else if (ctx.current_state == STATE_ERROR) {
        printf("Processing failed with error\n");
    }
    
    return 0;
}
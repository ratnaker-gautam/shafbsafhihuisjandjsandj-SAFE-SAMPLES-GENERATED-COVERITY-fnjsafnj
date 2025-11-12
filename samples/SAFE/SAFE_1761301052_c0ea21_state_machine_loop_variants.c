//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READY,
    PROCESSING,
    FINISHED,
    ERROR
};

struct context {
    enum state current_state;
    int data_value;
    int iteration_count;
    int max_iterations;
};

int validate_input(int value, int min, int max) {
    if (value < min || value > max) {
        return 0;
    }
    return 1;
}

void state_init(struct context *ctx) {
    ctx->current_state = READY;
    ctx->data_value = 0;
    ctx->iteration_count = 0;
    ctx->max_iterations = 5;
}

void state_ready(struct context *ctx) {
    int input_val;
    printf("Enter data value (1-100): ");
    
    if (scanf("%d", &input_val) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (!validate_input(input_val, 1, 100)) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->data_value = input_val;
    ctx->current_state = PROCESSING;
}

void state_processing(struct context *ctx) {
    int result;
    
    if (ctx->data_value <= 0) {
        ctx->current_state = ERROR;
        return;
    }
    
    result = ctx->data_value * 2;
    
    if (result < ctx->data_value) {
        ctx->current_state = ERROR;
        return;
    }
    
    printf("Processing: %d -> %d\n", ctx->data_value, result);
    ctx->data_value = result;
    ctx->iteration_count++;
    
    if (ctx->iteration_count >= ctx->max_iterations) {
        ctx->current_state = FINISHED;
    }
}

void state_finished(struct context *ctx) {
    printf("Final result: %d\n", ctx->data_value);
    printf("Completed %d iterations\n", ctx->iteration_count);
}

void state_error(struct context *ctx) {
    printf("Error state reached\n");
    printf("Last valid data: %d, Iterations: %d\n", 
           ctx->data_value, ctx->iteration_count);
}

int main(void) {
    struct context ctx;
    int running = 1;
    
    state_init(&ctx);
    
    while (running) {
        switch (ctx.current_state) {
            case INIT:
                state_init(&ctx);
                break;
            case READY:
                state_ready(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case FINISHED:
                state_finished(&ctx);
                running = 0;
                break;
            case ERROR:
                state_error(&ctx);
                running = 0;
                break;
            default:
                ctx.current_state = ERROR;
                break;
        }
        
        int counter = 0;
        do {
            counter++;
            if (counter > 1000) {
                ctx.current_state = ERROR;
                break;
            }
        } while (0);
        
        for (int i = 0; i < 1; i++) {
            if (ctx.current_state == ERROR || ctx.current_state == FINISHED) {
                break;
            }
        }
    }
    
    return 0;
}
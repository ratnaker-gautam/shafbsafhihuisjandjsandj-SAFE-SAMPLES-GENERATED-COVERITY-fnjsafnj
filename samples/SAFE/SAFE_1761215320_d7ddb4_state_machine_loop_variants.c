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
    printf("Enter initial value (1-100): ");
    
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
    int operation;
    printf("Current value: %d\n", ctx->data_value);
    printf("Choose operation (1=add 10, 2=multiply by 2, 3=subtract 5): ");
    
    if (scanf("%d", &operation) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    if (!validate_input(operation, 1, 3)) {
        ctx->current_state = ERROR;
        return;
    }
    
    switch (operation) {
        case 1:
            if (ctx->data_value <= 90) {
                ctx->data_value += 10;
            }
            break;
        case 2:
            if (ctx->data_value <= 50) {
                ctx->data_value *= 2;
            }
            break;
        case 3:
            if (ctx->data_value >= 6) {
                ctx->data_value -= 5;
            }
            break;
    }
    
    ctx->iteration_count++;
    
    if (ctx->iteration_count >= ctx->max_iterations) {
        ctx->current_state = FINISHED;
    }
}

void state_finished(struct context *ctx) {
    printf("Final result: %d\n", ctx->data_value);
    printf("Processing complete after %d iterations\n", ctx->iteration_count);
}

void state_error(struct context *ctx) {
    printf("Error occurred during processing\n");
    printf("Last valid state: %d, value: %d\n", ctx->current_state, ctx->data_value);
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
        }
    }
    
    return 0;
}
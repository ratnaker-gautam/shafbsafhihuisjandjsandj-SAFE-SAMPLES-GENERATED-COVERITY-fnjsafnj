//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    int step_count;
    int max_steps;
};

int validate_input(int value, int min, int max) {
    if (value < min || value > max) {
        return 0;
    }
    return 1;
}

void state_init(struct context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = READY;
    ctx->data_value = 0;
    ctx->step_count = 0;
    ctx->max_steps = 10;
    printf("State machine initialized\n");
}

void state_ready(struct context *ctx) {
    if (ctx == NULL) return;
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
    printf("Data accepted: %d\n", input_val);
}

void state_processing(struct context *ctx) {
    if (ctx == NULL) return;
    int i;
    int result;
    
    if (ctx->step_count >= ctx->max_steps) {
        ctx->current_state = FINISHED;
        return;
    }
    
    ctx->step_count++;
    
    for (i = 0; i < ctx->step_count; i++) {
        if (ctx->data_value > 0 && i > 0) {
            if (ctx->data_value > INT_MAX / i) {
                ctx->current_state = ERROR;
                return;
            }
            result = ctx->data_value * i;
        } else {
            result = ctx->data_value + i;
        }
        
        if (i == ctx->step_count - 1) {
            printf("Step %d: intermediate result = %d\n", ctx->step_count, result);
        }
    }
    
    if (ctx->step_count >= ctx->max_steps) {
        ctx->current_state = FINISHED;
    }
}

void state_finished(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Processing completed in %d steps\n", ctx->step_count);
    printf("Final data value: %d\n", ctx->data_value);
}

void state_error(struct context *ctx) {
    if (ctx == NULL) return;
    printf("Error state reached\n");
    printf("Last valid data: %d, Steps: %d\n", ctx->data_value, ctx->step_count);
}

int main(void) {
    struct context ctx;
    int run_count;
    
    printf("Enter number of state machine runs (1-5): ");
    if (scanf("%d", &run_count) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!validate_input(run_count, 1, 5)) {
        fprintf(stderr, "Run count out of range\n");
        return 1;
    }
    
    int run;
    for (run = 0; run < run_count; run++) {
        printf("\n--- Run %d ---\n", run + 1);
        
        state_init(&ctx);
        
        while (1) {
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
                    goto run_complete;
                case ERROR:
                    state_error(&ctx);
                    goto run_complete;
            }
            
            if (ctx.current_state == FINISHED || ctx.current_state == ERROR) {
                break;
            }
        }
        
        run_complete:
        if (run < run_count - 1) {
            printf("Press Enter to continue...");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            getchar();
        }
    }
    
    printf("\nAll state machine runs completed\n");
    return 0;
}
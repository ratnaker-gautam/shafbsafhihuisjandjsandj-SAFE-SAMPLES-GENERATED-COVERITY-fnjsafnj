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
    int counter;
    int max_iterations;
};

int validate_input(int argc, char *argv[]) {
    if (argc != 2) {
        return -1;
    }
    
    char *endptr;
    long val = strtol(argv[1], &endptr, 10);
    
    if (endptr == argv[1] || *endptr != '\0') {
        return -1;
    }
    
    if (val <= 0 || val > 1000) {
        return -1;
    }
    
    return (int)val;
}

void state_init(struct context *ctx) {
    ctx->current_state = READY;
    ctx->counter = 0;
}

void state_ready(struct context *ctx) {
    if (ctx->counter < ctx->max_iterations) {
        ctx->current_state = PROCESSING;
    } else {
        ctx->current_state = FINISHED;
    }
}

void state_processing(struct context *ctx) {
    if (ctx->counter >= ctx->max_iterations) {
        ctx->current_state = ERROR;
        return;
    }
    
    int result;
    int i = 0;
    int sum = 0;
    
    while (i < 10) {
        if (i + ctx->counter > 1000) {
            ctx->current_state = ERROR;
            return;
        }
        sum += i;
        i++;
    }
    
    for (int j = 0; j < 5; j++) {
        if (sum + j > 1000) {
            ctx->current_state = ERROR;
            return;
        }
        sum += j;
    }
    
    do {
        if (sum > 1000) {
            ctx->current_state = ERROR;
            return;
        }
        sum += 1;
    } while (sum < 50);
    
    ctx->counter++;
    
    if (ctx->counter <= ctx->max_iterations) {
        ctx->current_state = READY;
    } else {
        ctx->current_state = ERROR;
    }
}

void state_finished(struct context *ctx) {
    printf("Completed %d iterations successfully\n", ctx->counter);
}

void state_error(struct context *ctx) {
    printf("Error occurred at iteration %d\n", ctx->counter);
}

int main(int argc, char *argv[]) {
    struct context ctx;
    int max_iter = validate_input(argc, argv);
    
    if (max_iter == -1) {
        fprintf(stderr, "Usage: %s <iterations (1-1000)>\n", argv[0]);
        return 1;
    }
    
    ctx.max_iterations = max_iter;
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
                return 0;
            case ERROR:
                state_error(&ctx);
                return 1;
        }
    }
    
    return 0;
}
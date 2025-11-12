//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING_NUMBERS, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int numbers[100];
    int count;
    int sum;
};

int process_state(struct Context *ctx, int input);

int start_state(struct Context *ctx, int input) {
    if (input == 0) {
        ctx->current_state = FINISHED;
        return 1;
    }
    if (input > 0 && input <= 100) {
        ctx->current_state = READING_NUMBERS;
        ctx->count = 0;
        ctx->sum = 0;
        return process_state(ctx, input);
    }
    ctx->current_state = ERROR;
    return 0;
}

int reading_numbers_state(struct Context *ctx, int input) {
    if (input == -1) {
        if (ctx->count > 0) {
            ctx->current_state = PROCESSING;
            return process_state(ctx, 0);
        } else {
            ctx->current_state = ERROR;
            return 0;
        }
    }
    if (input < -1000 || input > 1000) {
        ctx->current_state = ERROR;
        return 0;
    }
    if (ctx->count >= 100) {
        ctx->current_state = ERROR;
        return 0;
    }
    ctx->numbers[ctx->count] = input;
    ctx->count++;
    return 1;
}

int processing_state(struct Context *ctx, int input) {
    if (input != 0) {
        ctx->current_state = ERROR;
        return 0;
    }
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->numbers[i];
    }
    ctx->current_state = FINISHED;
    return 1;
}

int finished_state(struct Context *ctx, int input) {
    return 0;
}

int error_state(struct Context *ctx, int input) {
    return 0;
}

int process_state(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return start_state(ctx, input);
        case READING_NUMBERS:
            return reading_numbers_state(ctx, input);
        case PROCESSING:
            return processing_state(ctx, input);
        case FINISHED:
            return finished_state(ctx, input);
        case ERROR:
            return error_state(ctx, input);
        default:
            ctx->current_state = ERROR;
            return 0;
    }
}

void reset_context(struct Context *ctx) {
    ctx->current_state = START;
    ctx->count = 0;
    ctx->sum = 0;
    memset(ctx->numbers, 0, sizeof(ctx->numbers));
}

int handle_input(struct Context *ctx) {
    int value;
    printf("Enter number (0 to finish, -1 to process, positive to start): ");
    if (scanf("%d", &value) != 1) {
        return -1;
    }
    return process_state(ctx, value);
}

void recursive_state_handler(struct Context *ctx, int depth) {
    if (depth >= 1000) {
        ctx->current_state = ERROR;
        return;
    }
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    if (!handle_input(ctx)) {
        return;
    }
    recursive_state_handler(ctx, depth + 1);
}

int main(void) {
    struct Context ctx;
    reset_context(&ctx);
    
    printf("Number Processor State Machine\n");
    printf("Enter positive number to start sequence\n");
    printf("Enter numbers to accumulate (-1 to process, 0 to exit)\n");
    
    recursive_state_handler(&ctx, 0);
    
    if (ctx.current_state == FINISHED) {
        printf("Sum of %d numbers: %d\n", ctx.count, ctx.sum);
    } else if (ctx.current_state == ERROR) {
        printf("Error occurred during processing\n");
    }
    
    return 0;
}
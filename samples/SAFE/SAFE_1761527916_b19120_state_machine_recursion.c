//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READING,
    PROCESSING,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int data[10];
    int count;
    int sum;
};

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->count = 0;
    ctx->sum = 0;
    return READING;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    if (ctx->count >= 10) {
        return PROCESSING;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    if (ctx->count == 10) {
        return PROCESSING;
    }
    return READING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    printf("Sum: %d\n", ctx->sum);
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == -1) {
        return START;
    }
    printf("Error state\n");
    return ERROR;
}

enum State process_state(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case FINISHED:
            return handle_finished(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            return ERROR;
    }
}

void state_machine_recursive(struct Context *ctx, int remaining) {
    if (remaining <= 0) {
        return;
    }
    
    int input;
    printf("Enter number (-1 to reset, 0 to process/finish): ");
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        ctx->current_state = ERROR;
    } else {
        ctx->current_state = process_state(ctx, input);
    }
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete\n");
        return;
    }
    
    state_machine_recursive(ctx, remaining - 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("State Machine Demo - Enter up to 10 numbers (0-100)\n");
    state_machine_recursive(&ctx, 15);
    
    if (ctx.current_state == ERROR) {
        printf("Terminated in error state\n");
        return 1;
    }
    
    return 0;
}
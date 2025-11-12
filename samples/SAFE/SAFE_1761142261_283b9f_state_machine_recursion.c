//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data[10];
    int count;
    int sum;
};

enum State handle_start(struct Context *ctx, int input) {
    if (input == 0) {
        printf("Starting state machine\n");
        return READING;
    } else {
        printf("Invalid start signal\n");
        return ERROR;
    }
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input == -1) {
        if (ctx->count > 0) {
            printf("Finished reading %d numbers\n", ctx->count);
            return PROCESSING;
        } else {
            printf("No numbers read\n");
            return ERROR;
        }
    } else if (ctx->count < 10) {
        ctx->data[ctx->count] = input;
        ctx->count++;
        printf("Read number: %d\n", input);
        return READING;
    } else {
        printf("Buffer full\n");
        return ERROR;
    }
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) {
        printf("Invalid processing input\n");
        return ERROR;
    }
    
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->sum > 0 && ctx->data[i] > 0 && ctx->sum + ctx->data[i] < ctx->sum) {
            printf("Integer overflow detected\n");
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    
    printf("Processing complete. Sum: %d\n", ctx->sum);
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 1) {
        printf("Resetting state machine\n");
        ctx->count = 0;
        ctx->sum = 0;
        return START;
    } else {
        printf("Invalid reset signal\n");
        return ERROR;
    }
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == -2) {
        printf("Recovering from error\n");
        ctx->count = 0;
        ctx->sum = 0;
        return START;
    } else {
        printf("Still in error state\n");
        return ERROR;
    }
}

void state_machine_recursive(struct Context *ctx, int input) {
    enum State next_state;
    
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx, input);
            break;
        case READING:
            next_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            next_state = handle_processing(ctx, input);
            break;
        case FINISHED:
            next_state = handle_finished(ctx, input);
            break;
        case ERROR:
            next_state = handle_error(ctx, input);
            break;
        default:
            next_state = ERROR;
            break;
    }
    
    ctx->current_state = next_state;
    
    if (next_state != FINISHED && next_state != ERROR) {
        int next_input;
        if (scanf("%d", &next_input) == 1) {
            state_machine_recursive(ctx, next_input);
        } else {
            printf("Invalid input format\n");
            ctx->current_state = ERROR;
        }
    }
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("State Machine Demo\n");
    printf("Commands: 0=start, numbers to read, -1=process, 1=reset, -2=recover\n");
    printf("Enter first command: ");
    
    int input;
    if (scanf("%d", &input) == 1) {
        state_machine_recursive(&ctx, input);
    } else {
        printf("Failed to read initial input\n");
        return 1;
    }
    
    if (ctx.current_state == ERROR) {
        printf("State machine ended in error state\n");
        return 1;
    }
    
    return 0;
}
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

enum State handle_start(struct Context *ctx, int input);
enum State handle_reading(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
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

enum State handle_start(struct Context *ctx, int input) {
    if (input == 0) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input == -1) {
        if (ctx->count > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    if (ctx->count >= 10) {
        return ERROR;
    }
    if (input < -100 || input > 100) {
        return ERROR;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    return READING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->sum > 1000 - ctx->data[i]) {
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 0) {
        printf("Sum: %d\n", ctx->sum);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("Error state encountered\n");
    return ERROR;
}

void state_machine_loop(struct Context *ctx, int input) {
    enum State next_state = transition(ctx, input);
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        state_machine_loop(ctx, 0);
    }
}

int main(void) {
    struct Context ctx = {START, {0}, 0, 0};
    
    printf("Enter numbers (0 to start, -1 to process, other to add): ");
    
    int value;
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            break;
        }
        state_machine_loop(&ctx, value);
    }
    
    if (ctx.current_state == ERROR) {
        printf("State machine terminated with error\n");
        return 1;
    }
    
    return 0;
}
//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step_count;
};

enum State handle_start(struct Context* ctx, int input);
enum State handle_processing(struct Context* ctx, int input);
enum State handle_finished(struct Context* ctx, int input);
enum State handle_error(struct Context* ctx, int input);

enum State transition(struct Context* ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
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

enum State handle_start(struct Context* ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->data = input;
    ctx->step_count = 0;
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    if (ctx->step_count >= 10) {
        return FINISHED;
    }
    if (input > ctx->data) {
        ctx->data = input;
    }
    ctx->step_count++;
    return PROCESSING;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    if (input == -1) {
        ctx->data = 0;
        ctx->step_count = 0;
        return START;
    }
    return ERROR;
}

void process_state_machine(struct Context* ctx, int input) {
    enum State next_state = transition(ctx, input);
    ctx->current_state = next_state;
    
    if (ctx->current_state == PROCESSING) {
        process_state_machine(ctx, input + 1);
    }
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    
    printf("Enter initial value (0-100): ");
    int initial_value;
    if (scanf("%d", &initial_value) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (initial_value < 0 || initial_value > 100) {
        printf("Value out of range\n");
        return 1;
    }
    
    process_state_machine(&ctx, initial_value);
    
    printf("Final state: ");
    switch (ctx.current_state) {
        case START:
            printf("START\n");
            break;
        case PROCESSING:
            printf("PROCESSING\n");
            break;
        case FINISHED:
            printf("FINISHED - Max value: %d\n", ctx.data);
            break;
        case ERROR:
            printf("ERROR\n");
            break;
    }
    
    return 0;
}
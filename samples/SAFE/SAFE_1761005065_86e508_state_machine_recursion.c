//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
    if (ctx == NULL) return ERROR;
    
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
    if (input < 0 || input > 100) return ERROR;
    
    ctx->data = input;
    ctx->step_count = 1;
    printf("State: START -> PROCESSING (data=%d)\n", input);
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (ctx->step_count >= 10) {
        printf("State: PROCESSING -> FINISHED (max steps reached)\n");
        return FINISHED;
    }
    
    if (input == 0) {
        printf("State: PROCESSING -> FINISHED (received stop signal)\n");
        return FINISHED;
    }
    
    if (input < -50 || input > 50) {
        printf("State: PROCESSING -> ERROR (invalid input range)\n");
        return ERROR;
    }
    
    if (input > 0) {
        if (ctx->data > INT_MAX - input) {
            printf("State: PROCESSING -> ERROR (integer overflow)\n");
            return ERROR;
        }
        ctx->data += input;
    } else {
        if (ctx->data < INT_MIN - input) {
            printf("State: PROCESSING -> ERROR (integer underflow)\n");
            return ERROR;
        }
        ctx->data += input;
    }
    
    ctx->step_count++;
    printf("State: PROCESSING -> PROCESSING (data=%d, step=%d)\n", ctx->data, ctx->step_count);
    return PROCESSING;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (input != 0) {
        printf("State: FINISHED -> ERROR (unexpected input after finish)\n");
        return ERROR;
    }
    printf("State: FINISHED -> FINISHED (final data=%d)\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    if (input == -1) {
        printf("State: ERROR -> START (reset)\n");
        return START;
    }
    printf("State: ERROR -> ERROR (data=%d)\n", ctx->data);
    return ERROR;
}

void run_state_machine(struct Context* ctx, int max_transitions) {
    if (ctx == NULL || max_transitions <= 0) return;
    
    if (max_transitions == 1) {
        int input;
        printf("Enter input for state %d: ", ctx->current_state);
        if (scanf("%d", &input) != 1) {
            ctx->current_state = ERROR;
            return;
        }
        ctx->current_state = transition(ctx, input);
        return;
    }
    
    int input;
    printf("Enter input for state %d: ", ctx->current_state);
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    enum State previous_state = ctx->current_state;
    ctx->current_state = transition(ctx, input);
    
    if (ctx->current_state != previous_state || ctx->current_state == PROCESSING) {
        run_state_machine(ctx, max_transitions - 1);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data = 0;
    ctx.step_count = 0;
    
    printf("State Machine Started (max 20 transitions)\n");
    run_state_machine(&ctx, 20);
    
    printf("Final state: %d, Final data: %d\n", ctx.current_state, ctx.data);
    return 0;
}
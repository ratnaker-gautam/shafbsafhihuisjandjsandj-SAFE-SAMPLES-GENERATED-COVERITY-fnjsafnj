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
    if (ctx == NULL) {
        return ERROR;
    }
    switch(ctx->current_state) {
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
    if (ctx == NULL) {
        return ERROR;
    }
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->data = input;
    ctx->step_count = 1;
    return PROCESSING;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (ctx == NULL) {
        return ERROR;
    }
    if (input < 0 || input > 100) {
        return ERROR;
    }
    
    if (ctx->step_count >= 10) {
        return FINISHED;
    }
    
    if (input > 0) {
        if (ctx->data > INT_MAX / 2) {
            return ERROR;
        }
        ctx->data *= 2;
    } else {
        if (ctx->data > INT_MAX - 10) {
            return ERROR;
        }
        ctx->data += 10;
    }
    
    ctx->step_count++;
    
    if (ctx->step_count >= 10) {
        return FINISHED;
    }
    return PROCESSING;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (ctx == NULL) {
        return ERROR;
    }
    if (input != 0) {
        return ERROR;
    }
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    return ERROR;
}

void process_state_machine(struct Context* ctx, int input) {
    if (ctx == NULL) {
        return;
    }
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
    
    if (ctx->current_state == PROCESSING) {
        process_state_machine(ctx, 0);
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
    
    process_state_machine(&ctx, initial_value);
    
    switch(ctx.current_state) {
        case FINISHED:
            printf("Processing completed. Final value: %d\n", ctx.data);
            break;
        case ERROR:
            printf("Error occurred during processing\n");
            break;
        default:
            printf("Unexpected state: %d\n", ctx.current_state);
            break;
    }
    
    return 0;
}
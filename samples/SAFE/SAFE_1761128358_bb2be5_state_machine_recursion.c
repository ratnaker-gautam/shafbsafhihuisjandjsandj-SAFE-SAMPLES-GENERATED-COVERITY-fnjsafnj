//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
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
        return ERROR;
    }
    
    if (input == 0) {
        return FINISHED;
    }
    
    ctx->data += input;
    ctx->step_count++;
    
    if (ctx->data > 1000) {
        return ERROR;
    }
    
    return PROCESSING;
}

enum State handle_finished(struct Context* ctx, int input) {
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    return ERROR;
}

void process_state_machine(struct Context* ctx, int input) {
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
    
    if (ctx->current_state == PROCESSING) {
        process_state_machine(ctx, 0);
    }
}

int main(void) {
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
    
    if (ctx.current_state == FINISHED) {
        printf("Processing completed successfully\n");
        printf("Final data value: %d\n", ctx.data);
        printf("Steps taken: %d\n", ctx.step_count);
    } else if (ctx.current_state == ERROR) {
        printf("Error occurred during processing\n");
        printf("Last data value: %d\n", ctx.data);
        return 1;
    } else {
        printf("Unexpected state: %d\n", ctx.current_state);
        return 1;
    }
    
    return 0;
}
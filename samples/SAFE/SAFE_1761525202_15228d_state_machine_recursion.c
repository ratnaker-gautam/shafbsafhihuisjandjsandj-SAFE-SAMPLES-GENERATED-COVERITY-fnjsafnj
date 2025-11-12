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

enum State handle_start(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
    if (ctx == NULL) return ERROR;
    if (ctx->step_count >= 100) return ERROR;
    
    ctx->step_count++;
    
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) return ERROR;
    
    ctx->data = input;
    printf("Starting with value: %d\n", input);
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 0) return ERROR;
    
    if (input > 0) {
        if (ctx->data > 1000000) return ERROR;
        ctx->data *= input;
        printf("Processing: multiplied to %d\n", ctx->data);
        
        if (ctx->data >= 1000) {
            return FINISHED;
        }
        return PROCESSING;
    } else {
        ctx->data += input;
        printf("Processing: added to %d\n", ctx->data);
        
        if (ctx->data <= 0) {
            return ERROR;
        }
        return PROCESSING;
    }
}

enum State handle_finished(struct Context *ctx, int input) {
    printf("Final result: %d\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("Error state reached\n");
    return ERROR;
}

void run_state_machine(struct Context *ctx, int input) {
    if (ctx == NULL) return;
    
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
    
    if (new_state != FINISHED && new_state != ERROR) {
        int next_input;
        if (scanf("%d", &next_input) == 1) {
            run_state_machine(ctx, next_input);
        } else {
            ctx->current_state = ERROR;
        }
    }
}

int main(void) {
    struct Context ctx = {START, 0, 0};
    int initial_input;
    
    printf("Enter initial value (0-100): ");
    if (scanf("%d", &initial_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    run_state_machine(&ctx, initial_input);
    
    if (ctx.current_state == FINISHED) {
        printf("State machine completed successfully\n");
    } else {
        printf("State machine terminated with error\n");
    }
    
    return 0;
}
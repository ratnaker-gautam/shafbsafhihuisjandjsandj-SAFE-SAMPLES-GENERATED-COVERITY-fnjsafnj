//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
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
    
    if (ctx->step_count < INT_MAX) {
        ctx->step_count++;
    }
    
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
    if (ctx == NULL) {
        return ERROR;
    }
    if (input == -1) {
        ctx->current_state = START;
        ctx->data = 0;
        ctx->step_count = 0;
        return START;
    }
    return ERROR;
}

void process_state_machine(struct Context* ctx, int input) {
    if (ctx == NULL) {
        return;
    }
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
}

void recursive_state_processor(struct Context* ctx, int remaining_steps) {
    if (ctx == NULL || remaining_steps <= 0) {
        return;
    }
    
    printf("Current state: %d, data: %d, step: %d\n", 
           ctx->current_state, ctx->data, ctx->step_count);
    printf("Enter input (-2 to exit, -1 to reset from error): ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        recursive_state_processor(ctx, remaining_steps);
        return;
    }
    
    if (input == -2) {
        return;
    }
    
    process_state_machine(ctx, input);
    
    if (ctx->current_state == FINISHED) {
        printf("Processing completed. Final data: %d\n", ctx->data);
        return;
    }
    
    if (ctx->current_state == ERROR) {
        printf("Error state reached\n");
    }
    
    if (remaining_steps > 1) {
        recursive_state_processor(ctx, remaining_steps - 1);
    }
}

int main(void) {
    struct Context ctx = {START, 0, 0};
    
    printf("State Machine Demo (max 20 steps)\n");
    printf("Valid inputs: 0-100 (normal), -1 (reset), -2 (exit)\n");
    
    recursive_state_processor(&ctx, 20);
    
    printf("Program terminated\n");
    return 0;
}
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

enum State handle_start(struct Context* ctx, int input);
enum State handle_reading(struct Context* ctx, int input);
enum State handle_processing(struct Context* ctx, int input);
enum State handle_finished(struct Context* ctx, int input);
enum State handle_error(struct Context* ctx, int input);

enum State transition(struct Context* ctx, int input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context* ctx, int input) {
    if (input == 1) {
        ctx->count = 0;
        ctx->sum = 0;
        printf("State machine started. Enter numbers (0 to finish):\n");
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, int input) {
    if (input == 0) {
        if (ctx->count > 0) {
            return PROCESSING;
        } else {
            printf("No numbers entered.\n");
            return ERROR;
        }
    }
    if (ctx->count >= 10) {
        printf("Maximum numbers reached.\n");
        return PROCESSING;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    return READING;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input != 0) return ERROR;
    
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    printf("Processing complete. Sum: %d\n", ctx->sum);
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (input == 2) {
        printf("Resetting state machine.\n");
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    if (input == 2) {
        printf("Resetting from error state.\n");
        return START;
    }
    return ERROR;
}

void run_state_machine(struct Context* ctx, int input) {
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("State Machine Demo\n");
    printf("1 - Start, 0 - Process/Finish, 2 - Reset, other - Error/Continue\n");
    
    int input;
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Current state: %d > ", ctx.current_state);
        if (scanf("%d", &input) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            ctx.current_state = ERROR;
            continue;
        }
        run_state_machine(&ctx, input);
    }
    
    if (ctx.current_state == ERROR) {
        printf("Entered error state. Press 2 to reset.\n");
        while (ctx.current_state == ERROR) {
            printf("> ");
            if (scanf("%d", &input) != 1) {
                while (getchar() != '\n');
                continue;
            }
            run_state_machine(&ctx, input);
        }
    }
    
    return 0;
}
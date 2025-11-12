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

enum State process_state(struct Context* ctx, int input) {
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
    if (input == 0) {
        ctx->count = 0;
        ctx->sum = 0;
        printf("Starting data collection\n");
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, int input) {
    if (input == -1) {
        if (ctx->count > 0) {
            printf("Finished reading %d numbers\n", ctx->count);
            return PROCESSING;
        }
        return ERROR;
    }
    if (ctx->count >= 10) {
        return ERROR;
    }
    if (input < -1000 || input > 1000) {
        return ERROR;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    printf("Read number: %d\n", input);
    return READING;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input != 0) {
        return ERROR;
    }
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->sum > 1000000 - ctx->data[i] || ctx->sum < -1000000 - ctx->data[i]) {
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    printf("Processing complete. Sum: %d\n", ctx->sum);
    return FINISHED;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (input == 1) {
        printf("Resetting state machine\n");
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    if (input == 1) {
        printf("Resetting from error state\n");
        return START;
    }
    return ERROR;
}

void run_state_machine(struct Context* ctx, int input) {
    enum State new_state = process_state(ctx, input);
    ctx->current_state = new_state;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("State Machine Demo\n");
    printf("Enter numbers to collect (0 to start, -1 to process, 1 to reset)\n");
    
    int input;
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Current state: %d, Enter input: ", ctx.current_state);
        if (scanf("%d", &input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            ctx.current_state = ERROR;
            continue;
        }
        run_state_machine(&ctx, input);
    }
    
    if (ctx.current_state == ERROR) {
        printf("Enter 1 to reset: ");
        if (scanf("%d", &input) == 1 && input == 1) {
            run_state_machine(&ctx, 1);
        }
    }
    
    return 0;
}
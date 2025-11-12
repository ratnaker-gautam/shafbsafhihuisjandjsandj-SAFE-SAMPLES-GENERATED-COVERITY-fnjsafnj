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

enum State handle_start(struct Context* ctx, int input) {
    if (input == 1) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context* ctx, int input) {
    if (input >= 0 && input <= 100) {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            if (ctx->count == 10) {
                return PROCESSING;
            }
            return READING;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context* ctx, int input) {
    if (input == 0) {
        for (int i = 0; i < ctx->count; i++) {
            ctx->sum += ctx->data[i];
        }
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context* ctx, int input) {
    if (input == -1) {
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
    if (input == -1) {
        return START;
    }
    return ERROR;
}

void process_state(struct Context* ctx, int input) {
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx, input);
            break;
        case READING:
            ctx->current_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            ctx->current_state = handle_processing(ctx, input);
            break;
        case FINISHED:
            ctx->current_state = handle_finished(ctx, input);
            break;
        case ERROR:
            ctx->current_state = handle_error(ctx, input);
            break;
    }
}

void run_state_machine(struct Context* ctx) {
    int input;
    printf("Enter input (-2 to exit): ");
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        ctx->current_state = ERROR;
        return;
    }
    
    if (input == -2) {
        return;
    }
    
    process_state(ctx, input);
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
    } else if (ctx->current_state == ERROR) {
        printf("Error state reached. Enter -1 to reset.\n");
    }
    
    run_state_machine(ctx);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("State Machine Started\n");
    printf("Instructions:\n");
    printf("Start: Enter 1 to begin reading\n");
    printf("Reading: Enter numbers 0-100 (max 10)\n");
    printf("Processing: Enter 0 to calculate sum\n");
    printf("Finished: Enter -1 to restart\n");
    printf("Error: Enter -1 to restart\n");
    printf("Exit: Enter -2 to quit\n");
    
    run_state_machine(&ctx);
    
    printf("State Machine Terminated\n");
    return 0;
}
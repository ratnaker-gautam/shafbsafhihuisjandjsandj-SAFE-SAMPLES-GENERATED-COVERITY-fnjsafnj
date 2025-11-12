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

enum State handle_start(struct Context *ctx, int input) {
    if (input == 1) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input >= 0 && input <= 100) {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            return READING;
        } else {
            return PROCESSING;
        }
    } else if (input == -1) {
        if (ctx->count > 0) {
            return PROCESSING;
        } else {
            return ERROR;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->count > 0 && ctx->count <= 10) {
        for (int i = 0; i < ctx->count; i++) {
            if (ctx->data[i] >= 0 && ctx->data[i] <= 100) {
                ctx->sum += ctx->data[i];
            } else {
                return ERROR;
            }
        }
        return FINISHED;
    }
    return ERROR;
}

void process_state_machine(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            ctx->current_state = handle_start(ctx, input);
            break;
        case READING:
            ctx->current_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            ctx->current_state = handle_processing(ctx);
            break;
        case FINISHED:
            return;
        case ERROR:
            return;
    }
    
    if (ctx->current_state == PROCESSING) {
        process_state_machine(ctx, 0);
    } else if (ctx->current_state == READING) {
        printf("Enter number (0-100) or -1 to finish: ");
        int new_input;
        if (scanf("%d", &new_input) == 1) {
            process_state_machine(ctx, new_input);
        } else {
            ctx->current_state = ERROR;
        }
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("Enter 1 to start: ");
    int initial_input;
    if (scanf("%d", &initial_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    process_state_machine(&ctx, initial_input);
    
    if (ctx.current_state == FINISHED) {
        printf("Sum of %d numbers: %d\n", ctx.count, ctx.sum);
        return 0;
    } else {
        printf("Error occurred\n");
        return 1;
    }
}
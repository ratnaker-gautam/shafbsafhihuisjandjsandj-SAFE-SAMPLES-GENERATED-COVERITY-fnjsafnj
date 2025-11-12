//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current;
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
            if (ctx->count == 10) {
                return PARSING;
            }
            return READING;
        }
    }
    return ERROR;
}

enum State handle_parsing(struct Context *ctx, int input) {
    if (input == 2) {
        for (int i = 0; i < ctx->count; i++) {
            ctx->sum += ctx->data[i];
        }
        return PROCESSING;
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 3) {
        if (ctx->sum > 0) {
            return FINISHED;
        }
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, int input) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    return ERROR;
}

enum State (*handlers[])(struct Context *, int) = {
    handle_start,
    handle_reading,
    handle_parsing,
    handle_processing,
    handle_finished,
    handle_error
};

void process_state(struct Context *ctx, int input) {
    enum State next = handlers[ctx->current](ctx, input);
    ctx->current = next;
}

void run_state_machine(struct Context *ctx) {
    int input;
    while (ctx->current != FINISHED && ctx->current != ERROR) {
        printf("Enter input (0-100 for data, 1=start, 2=parse, 3=process): ");
        if (scanf("%d", &input) != 1) {
            ctx->current = ERROR;
            break;
        }
        process_state(ctx, input);
    }
    
    if (ctx->current == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
    } else {
        printf("Error occurred during processing.\n");
    }
}

int main(void) {
    struct Context ctx;
    ctx.current = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    run_state_machine(&ctx);
    
    return 0;
}
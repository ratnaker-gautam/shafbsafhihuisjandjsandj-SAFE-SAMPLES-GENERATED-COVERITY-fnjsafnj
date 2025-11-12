//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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
            return READING;
        } else {
            return PROCESSING;
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
    return FINISHED;
}

enum State handle_error(struct Context* ctx, int input) {
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
    
    if (ctx->current_state == READING || ctx->current_state == PROCESSING) {
        process_state(ctx, -1);
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("Enter 1 to start data collection: ");
    int input;
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    process_state(&ctx, input);
    
    if (ctx.current_state == READING) {
        printf("Enter up to 10 numbers (0-100), one per line:\n");
        for (int i = 0; i < 10; i++) {
            if (scanf("%d", &input) != 1) {
                ctx.current_state = ERROR;
                break;
            }
            process_state(&ctx, input);
            if (ctx.current_state != READING) {
                break;
            }
        }
    }
    
    if (ctx.current_state == PROCESSING) {
        printf("Enter 0 to process data: ");
        if (scanf("%d", &input) != 1) {
            ctx.current_state = ERROR;
        } else {
            process_state(&ctx, input);
        }
    }
    
    if (ctx.current_state == FINISHED) {
        printf("Data processed successfully.\n");
        printf("Numbers entered: %d\n", ctx.count);
        printf("Sum: %d\n", ctx.sum);
        return 0;
    } else {
        printf("Error occurred during processing.\n");
        return 1;
    }
}
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
            if (ctx->sum + ctx->data[i] > 1000) {
                return ERROR;
            }
            ctx->sum += ctx->data[i];
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
            break;
        case ERROR:
            break;
    }
    
    if (ctx->current_state == READING || ctx->current_state == START) {
        int next_input;
        if (scanf("%d", &next_input) == 1) {
            process_state_machine(ctx, next_input);
        } else {
            ctx->current_state = ERROR;
        }
    }
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("Enter 1 to start data collection\n");
    printf("Then enter numbers 0-100 (max 10), or -1 to finish early\n");
    
    int first_input;
    if (scanf("%d", &first_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    process_state_machine(&ctx, first_input);
    
    if (ctx.current_state == FINISHED) {
        printf("Successfully processed %d numbers\n", ctx.count);
        printf("Sum: %d\n", ctx.sum);
        return 0;
    } else {
        printf("Error occurred during processing\n");
        return 1;
    }
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    READING,
    PROCESSING,
    FINISHED,
    ERROR
};

struct Context {
    enum State current_state;
    int data[10];
    int count;
    int sum;
};

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    ctx->count = 0;
    ctx->sum = 0;
    return READING;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input < 0 || input > 100) {
        return ERROR;
    }
    if (input == 0) {
        if (ctx->count == 0) {
            return ERROR;
        }
        return PROCESSING;
    }
    if (ctx->count >= 10) {
        return ERROR;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    return READING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->count <= 0 || ctx->count > 10) {
        return ERROR;
    }
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->data[i] < 0 || ctx->data[i] > 100) {
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx) {
    printf("Sum: %d\n", ctx->sum);
    return FINISHED;
}

enum State handle_error(void) {
    printf("Error: Invalid input or state\n");
    return ERROR;
}

void state_machine(struct Context *ctx, int input) {
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
            ctx->current_state = handle_finished(ctx);
            break;
        case ERROR:
            ctx->current_state = handle_error();
            break;
    }
    
    if (ctx->current_state == READING || ctx->current_state == START) {
        int next_input;
        if (scanf("%d", &next_input) != 1) {
            ctx->current_state = ERROR;
            return;
        }
        state_machine(ctx, next_input);
    } else if (ctx->current_state == PROCESSING) {
        state_machine(ctx, 0);
    }
}

int main(void) {
    struct Context ctx = {START, {0}, 0, 0};
    
    printf("Enter numbers (0 to finish, max 10 numbers, 0-100 range):\n");
    int first_input;
    if (scanf("%d", &first_input) != 1) {
        printf("Error: Invalid input\n");
        return 1;
    }
    
    state_machine(&ctx, first_input);
    
    if (ctx.current_state == ERROR) {
        return 1;
    }
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data[10];
    int data_count;
    int sum;
    int index;
};

enum State handle_start(struct Context *ctx, int input) {
    if (input == 1) {
        ctx->data_count = 0;
        ctx->sum = 0;
        ctx->index = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input >= 0 && input <= 100) {
        if (ctx->data_count < 10) {
            ctx->data[ctx->data_count] = input;
            ctx->data_count++;
            return READING;
        } else {
            return PARSING;
        }
    } else if (input == -1) {
        if (ctx->data_count > 0) {
            return PARSING;
        } else {
            return ERROR;
        }
    }
    return ERROR;
}

enum State handle_parsing(struct Context *ctx, int input) {
    if (input == 0) {
        if (ctx->index < ctx->data_count) {
            ctx->sum += ctx->data[ctx->index];
            ctx->index++;
            return PROCESSING;
        } else {
            return FINISHED;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 0) {
        return PARSING;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, int input) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    return ERROR;
}

enum State process_input(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PARSING: return handle_parsing(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

void run_state_machine(struct Context *ctx, int input) {
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    enum State new_state = process_input(ctx, input);
    ctx->current_state = new_state;
    
    if (new_state == PARSING || new_state == PROCESSING) {
        run_state_machine(ctx, 0);
    }
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("State Machine: Data Sum Calculator\n");
    printf("Enter 1 to start, then numbers 0-100, -1 to finish reading\n");
    
    int input;
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        if (scanf("%d", &input) != 1) {
            ctx.current_state = ERROR;
            break;
        }
        
        run_state_machine(&ctx, input);
        
        if (ctx.current_state == ERROR) {
            printf("Invalid state transition\n");
            break;
        }
    }
    
    if (ctx.current_state == FINISHED) {
        printf("Sum of %d numbers: %d\n", ctx.data_count, ctx.sum);
    } else {
        printf("State machine terminated with error\n");
    }
    
    return 0;
}
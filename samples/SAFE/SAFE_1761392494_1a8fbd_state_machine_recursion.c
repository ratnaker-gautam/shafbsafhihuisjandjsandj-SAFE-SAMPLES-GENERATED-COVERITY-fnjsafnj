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
    int processed_count;
};

enum State handle_start(struct Context *ctx, int input) {
    if (input == 1) {
        ctx->data_count = 0;
        ctx->sum = 0;
        ctx->processed_count = 0;
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
        if (ctx->data_count > 0) {
            ctx->sum = 0;
            ctx->processed_count = 0;
            return PROCESSING;
        } else {
            return ERROR;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 0) {
        if (ctx->processed_count < ctx->data_count) {
            ctx->sum += ctx->data[ctx->processed_count];
            ctx->processed_count++;
            return PROCESSING;
        } else {
            return FINISHED;
        }
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
        case PARSING:
            ctx->current_state = handle_parsing(ctx, input);
            break;
        case PROCESSING:
            ctx->current_state = handle_processing(ctx, input);
            break;
        case FINISHED:
            break;
        case ERROR:
            break;
    }
}

void recursive_process(struct Context *ctx, int depth) {
    if (depth <= 0 || ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    printf("Enter input for state %d: ", ctx->current_state);
    int input;
    if (scanf("%d", &input) != 1) {
        ctx->current_state = ERROR;
        return;
    }
    
    process_state_machine(ctx, input);
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
        return;
    } else if (ctx->current_state == ERROR) {
        printf("State machine error occurred.\n");
        return;
    }
    
    recursive_process(ctx, depth - 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_count = 0;
    ctx.sum = 0;
    ctx.processed_count = 0;
    
    printf("State Machine: Process integer data\n");
    printf("Instructions:\n");
    printf("1. Enter '1' to start reading data\n");
    printf("2. Enter numbers 0-100 to add data (max 10)\n");
    printf("3. Enter '-1' to finish reading\n");
    printf("4. Enter '0' to start processing\n");
    printf("5. Enter '0' repeatedly to process each element\n");
    
    recursive_process(&ctx, 20);
    
    if (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("State machine did not complete properly.\n");
    }
    
    return 0;
}
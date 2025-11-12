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
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

void process_state(struct Context *ctx, int input) {
    enum State next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx, input);
            break;
        case READING:
            next_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            next_state = handle_processing(ctx);
            break;
        case FINISHED:
            return;
        case ERROR:
            return;
    }
    
    ctx->current_state = next_state;
    
    if (next_state != FINISHED && next_state != ERROR) {
        process_state(ctx, input);
    }
}

int get_valid_input(void) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -2;
    }
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -2;
    }
    
    if (value < -1 || value > 100) {
        return -2;
    }
    
    return (int)value;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    printf("State Machine: Number Processor\n");
    printf("Enter 1 to start, then numbers 0-100, -1 to finish\n");
    
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        if (ctx.current_state == START || ctx.current_state == READING) {
            printf("Current state: %d, Enter input: ", ctx.current_state);
            int input = get_valid_input();
            if (input == -2) {
                ctx.current_state = ERROR;
                break;
            }
            process_state(&ctx, input);
        } else if (ctx.current_state == PROCESSING) {
            process_state(&ctx, 0);
        }
    }
    
    if (ctx.current_state == FINISHED) {
        printf("Processing complete. Sum of %d numbers: %d\n", ctx.count, ctx.sum);
        return 0;
    } else {
        printf("Error occurred during processing.\n");
        return 1;
    }
}
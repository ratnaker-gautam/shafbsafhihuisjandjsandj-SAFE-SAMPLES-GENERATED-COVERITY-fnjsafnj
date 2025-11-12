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

enum State handle_processing(struct Context* ctx) {
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->sum > 1000 - ctx->data[i]) {
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

void process_state_machine(struct Context* ctx, int input) {
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
        case ERROR:
            return;
    }
    
    if (ctx->current_state == PROCESSING) {
        process_state_machine(ctx, 0);
    }
}

int get_valid_input(void) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -2;
    }
    
    char* endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return -2;
    }
    
    if (val < -1 || val > 100) {
        return -2;
    }
    
    if (val < -1 || val > 100) {
        return -2;
    }
    
    return (int)val;
}

int main(void) {
    struct Context ctx = {START, {0}, 0, 0};
    
    printf("Enter 1 to start data collection\n");
    
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        int input = get_valid_input();
        if (input == -2) {
            printf("Invalid input. Enter numbers 0-100 or -1 to finish\n");
            continue;
        }
        
        process_state_machine(&ctx, input);
        
        switch (ctx.current_state) {
            case READING:
                printf("Enter number %d (0-100) or -1 to finish: ", ctx.count + 1);
                break;
            case PROCESSING:
                printf("Processing data...\n");
                break;
            case FINISHED:
                printf("Sum of %d numbers: %d\n", ctx.count, ctx.sum);
                break;
            case ERROR:
                printf("Error occurred in state machine\n");
                break;
            default:
                break;
        }
    }
    
    return ctx.current_state == FINISHED ? 0 : 1;
}
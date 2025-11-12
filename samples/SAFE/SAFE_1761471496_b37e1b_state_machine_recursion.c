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
            if (ctx->count == 10) {
                return PROCESSING;
            }
            return READING;
        }
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 0) {
        for (int i = 0; i < ctx->count; i++) {
            ctx->sum += ctx->data[i];
        }
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == -1) {
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == -1) {
        return START;
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

void recursive_state_processor(struct Context *ctx, int iteration) {
    if (iteration >= 20) {
        return;
    }
    
    printf("Current state: %d, Enter input: ", ctx->current_state);
    
    int input;
    if (scanf("%d", &input) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        recursive_state_processor(ctx, iteration + 1);
        return;
    }
    
    process_state_machine(ctx, input);
    
    if (ctx->current_state == FINISHED) {
        printf("Processing complete. Sum: %d\n", ctx->sum);
        printf("Enter -1 to restart or any other number to stay in finished state: ");
    } else if (ctx->current_state == ERROR) {
        printf("Error state. Enter -1 to restart: ");
    }
    
    recursive_state_processor(ctx, iteration + 1);
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    printf("State Machine Demo\n");
    printf("Instructions:\n");
    printf("START: Enter 1 to begin reading numbers\n");
    printf("READING: Enter numbers 0-100 (max 10 numbers)\n");
    printf("PROCESSING: Enter 0 to calculate sum\n");
    printf("FINISHED: Enter -1 to restart\n");
    printf("ERROR: Enter -1 to restart\n");
    printf("Maximum 20 iterations\n\n");
    
    recursive_state_processor(&ctx, 0);
    
    printf("Program terminated after 20 iterations\n");
    return 0;
}
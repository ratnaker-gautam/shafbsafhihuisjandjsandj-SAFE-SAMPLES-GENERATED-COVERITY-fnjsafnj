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
            if (ctx->data_count == 10) {
                return PARSING;
            }
            return READING;
        }
    }
    return ERROR;
}

enum State handle_parsing(struct Context *ctx, int input) {
    if (input == 2) {
        ctx->sum = 0;
        for (int i = 0; i < ctx->data_count; i++) {
            ctx->sum += ctx->data[i];
        }
        return PROCESSING;
    }
    return ERROR;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input == 3) {
        if (ctx->processed_count < ctx->data_count) {
            ctx->data[ctx->processed_count] *= 2;
            ctx->processed_count++;
            if (ctx->processed_count == ctx->data_count) {
                return FINISHED;
            }
            return PROCESSING;
        }
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 4) {
        for (int i = 0; i < ctx->data_count; i++) {
            printf("%d ", ctx->data[i]);
        }
        printf("\nSum: %d\n", ctx->sum);
        return START;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == 0) {
        return START;
    }
    return ERROR;
}

enum State process_state(struct Context *ctx, int input) {
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

void state_machine_recursive(struct Context *ctx, int max_depth) {
    if (max_depth <= 0) {
        return;
    }
    
    printf("Current state: %d\n", ctx->current_state);
    printf("Enter input (-1 to exit): ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        ctx->current_state = ERROR;
    } else {
        if (input == -1) {
            return;
        }
        ctx->current_state = process_state(ctx, input);
    }
    
    state_machine_recursive(ctx, max_depth - 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.data_count = 0;
    ctx.sum = 0;
    ctx.processed_count = 0;
    
    printf("State Machine Demo\n");
    printf("Instructions:\n");
    printf("1: Start reading numbers (0-100)\n");
    printf("2: Parse and calculate sum\n");
    printf("3: Process data (double values)\n");
    printf("4: Display results\n");
    printf("0: Reset from error state\n");
    printf("-1: Exit\n");
    
    state_machine_recursive(&ctx, 100);
    
    return 0;
}
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

enum State handle_start(struct Context *ctx, int input);
enum State handle_reading(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PROCESSING: return handle_processing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context *ctx, int input) {
    if (input == 1) {
        ctx->count = 0;
        ctx->sum = 0;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input == 0) {
        if (ctx->count > 0) {
            return PROCESSING;
        }
        return ERROR;
    }
    if (ctx->count >= 10) {
        return ERROR;
    }
    ctx->data[ctx->count] = input;
    ctx->count++;
    return READING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 2) {
        return ERROR;
    }
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->sum > 1000 - ctx->data[i]) {
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 3) {
        printf("Sum: %d\n", ctx->sum);
        return FINISHED;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("Invalid state transition\n");
    return ERROR;
}

void run_state_machine(struct Context *ctx, int *inputs, int input_count, int depth) {
    if (depth >= input_count) {
        return;
    }
    if (depth < 0 || depth >= 100) {
        return;
    }
    ctx->current_state = transition(ctx, inputs[depth]);
    run_state_machine(ctx, inputs, input_count, depth + 1);
}

int main() {
    struct Context ctx = {START, {0}, 0, 0};
    int inputs[20];
    int input_count = 0;
    char buffer[100];
    
    printf("Enter inputs (0-3, one per line, max 20, empty to finish):\n");
    while (input_count < 20) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        if (buffer[0] == '\n') {
            break;
        }
        char *endptr;
        long val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input\n");
            return 1;
        }
        if (val < 0 || val > 3) {
            printf("Input out of range\n");
            return 1;
        }
        inputs[input_count] = (int)val;
        input_count++;
    }
    
    if (input_count == 0) {
        printf("No inputs provided\n");
        return 1;
    }
    
    run_state_machine(&ctx, inputs, input_count, 0);
    
    return 0;
}
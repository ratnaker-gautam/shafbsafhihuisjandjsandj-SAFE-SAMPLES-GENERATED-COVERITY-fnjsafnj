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
    if (input == 0) {
        printf("Starting state machine\n");
        return READING;
    } else {
        printf("Invalid start signal\n");
        return ERROR;
    }
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input < 0) {
        printf("Negative input detected\n");
        return PROCESSING;
    } else if (input > 0) {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            printf("Added value: %d\n", input);
            return READING;
        } else {
            printf("Buffer full\n");
            return PROCESSING;
        }
    } else {
        printf("Zero input detected\n");
        return PROCESSING;
    }
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) {
        printf("Invalid processing input\n");
        return ERROR;
    }
    
    if (ctx->count == 0) {
        printf("No data to process\n");
        return ERROR;
    }
    
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->sum > 1000 - ctx->data[i]) {
            printf("Sum overflow detected\n");
            return ERROR;
        }
        ctx->sum += ctx->data[i];
    }
    
    printf("Processed %d numbers, sum: %d\n", ctx->count, ctx->sum);
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 0) {
        printf("Machine finished successfully\n");
        return FINISHED;
    } else {
        printf("Invalid finished state input\n");
        return ERROR;
    }
}

enum State handle_error(struct Context *ctx, int input) {
    printf("Machine in error state\n");
    return ERROR;
}

enum State process_state(struct Context *ctx, int input) {
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PROCESSING:
            return handle_processing(ctx, input);
        case FINISHED:
            return handle_finished(ctx, input);
        case ERROR:
            return handle_error(ctx, input);
        default:
            return ERROR;
    }
}

void run_state_machine(struct Context *ctx, int *inputs, int input_count, int current_index) {
    if (current_index >= input_count) {
        return;
    }
    
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) {
        return;
    }
    
    enum State next_state = process_state(ctx, inputs[current_index]);
    ctx->current_state = next_state;
    
    run_state_machine(ctx, inputs, input_count, current_index + 1);
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.count = 0;
    ctx.sum = 0;
    
    int inputs[20];
    int input_count = 0;
    char buffer[100];
    
    printf("Enter up to 20 integers (one per line, empty line to stop):\n");
    
    while (input_count < 20) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (buffer[0] == '\n') {
            break;
        }
        
        char *endptr;
        long value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input, stopping\n");
            break;
        }
        
        if (value < -1000 || value > 1000) {
            printf("Input out of range, stopping\n");
            break;
        }
        
        inputs[input_count] = (int)value;
        input_count++;
    }
    
    if (input_count == 0) {
        printf("No inputs provided\n");
        return 1;
    }
    
    run_state_machine(&ctx, inputs, input_count, 0);
    
    if (ctx.current_state == ERROR) {
        printf("State machine ended in error state\n");
        return 1;
    } else if (ctx.current_state == FINISHED) {
        printf("State machine completed successfully\n");
        return 0;
    } else {
        printf("State machine ended in unexpected state\n");
        return 1;
    }
}
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
        return READING;
    } else if (input > 0 && input < 100) {
        ctx->data[0] = input;
        ctx->count = 1;
        return PROCESSING;
    } else {
        return ERROR;
    }
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input < 0) {
        return PROCESSING;
    } else if (input >= 0 && input < 100 && ctx->count < 10) {
        ctx->data[ctx->count] = input;
        ctx->count++;
        return READING;
    } else {
        return ERROR;
    }
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->count <= 0 || ctx->count > 10) {
        return ERROR;
    }
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    return FINISHED;
}

enum State handle_finished(struct Context *ctx) {
    printf("Sum: %d\n", ctx->sum);
    return FINISHED;
}

enum State handle_error(void) {
    printf("Error state reached\n");
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
}

void process_inputs(struct Context *ctx, int inputs[], int num_inputs, int index) {
    if (index >= num_inputs) {
        return;
    }
    
    state_machine(ctx, inputs[index]);
    
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    process_inputs(ctx, inputs, num_inputs, index + 1);
}

int main(void) {
    struct Context ctx = {START, {0}, 0, 0};
    
    int inputs[20];
    int input_count = 0;
    char buffer[32];
    
    printf("Enter numbers (negative to process, non-number to stop):\n");
    
    while (input_count < 20) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        char *endptr;
        long value = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer) {
            break;
        }
        
        if (value < -1000 || value > 1000) {
            printf("Input out of range\n");
            continue;
        }
        
        inputs[input_count] = (int)value;
        input_count++;
    }
    
    if (input_count == 0) {
        printf("No valid inputs provided\n");
        return 1;
    }
    
    process_inputs(&ctx, inputs, input_count, 0);
    
    return 0;
}
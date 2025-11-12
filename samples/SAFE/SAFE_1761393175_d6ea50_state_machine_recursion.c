//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data;
    int step;
};

enum State handle_start(struct Context *ctx, int input);
enum State handle_processing(struct Context *ctx, int input);
enum State handle_finished(struct Context *ctx, int input);
enum State handle_error(struct Context *ctx, int input);

enum State transition(struct Context *ctx, int input) {
    if (ctx == NULL) return ERROR;
    
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
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

enum State handle_start(struct Context *ctx, int input) {
    if (input < 0 || input > 100) return ERROR;
    
    ctx->data = input;
    ctx->step = 1;
    printf("State: START -> PROCESSING (data=%d)\n", ctx->data);
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input < 0 || input > 100) return ERROR;
    
    if (ctx->step >= 3) {
        printf("State: PROCESSING -> FINISHED (final=%d)\n", ctx->data);
        return FINISHED;
    }
    
    ctx->data += input;
    ctx->step++;
    printf("State: PROCESSING (step=%d, data=%d)\n", ctx->step, ctx->data);
    
    if (ctx->data > 1000) return ERROR;
    
    return PROCESSING;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input != 0) return ERROR;
    printf("State: FINISHED (result=%d)\n", ctx->data);
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    printf("State: ERROR\n");
    return ERROR;
}

void state_machine_recursive(struct Context *ctx, int remaining_inputs, int *inputs) {
    if (ctx == NULL || inputs == NULL) return;
    if (remaining_inputs <= 0) return;
    
    int input = inputs[0];
    if (input < -100 || input > 100) {
        ctx->current_state = ERROR;
        return;
    }
    
    ctx->current_state = transition(ctx, input);
    
    if (ctx->current_state == FINISHED || ctx->current_state == ERROR) {
        return;
    }
    
    state_machine_recursive(ctx, remaining_inputs - 1, inputs + 1);
}

int main(void) {
    struct Context ctx = {START, 0, 0};
    
    int inputs[10];
    int input_count = 0;
    
    printf("Enter up to 10 integers between 0-100 (or -1 to stop early):\n");
    
    for (int i = 0; i < 10; i++) {
        char buffer[32];
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        
        int num;
        if (sscanf(buffer, "%d", &num) != 1) {
            printf("Invalid input. Using 0.\n");
            num = 0;
        }
        
        if (num == -1) break;
        
        if (num < 0 || num > 100) {
            printf("Input out of range. Using 0.\n");
            num = 0;
        }
        
        inputs[input_count] = num;
        input_count++;
        
        if (input_count >= 10) break;
    }
    
    if (input_count == 0) {
        printf("No valid inputs provided.\n");
        return 1;
    }
    
    state_machine_recursive(&ctx, input_count, inputs);
    
    if (ctx.current_state == FINISHED) {
        printf("Final result: %d\n", ctx.data);
    } else {
        printf("Terminated in error state.\n");
    }
    
    return 0;
}
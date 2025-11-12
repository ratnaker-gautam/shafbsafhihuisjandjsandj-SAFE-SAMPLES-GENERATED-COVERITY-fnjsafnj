//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    int data[10];
    int count;
    int sum;
};

int state_machine(struct Context *ctx, int input_value) {
    if (ctx == NULL) return -1;
    
    switch (ctx->current_state) {
        case START:
            if (input_value >= 0) {
                ctx->current_state = READING;
                ctx->count = 0;
                ctx->sum = 0;
                return state_machine(ctx, input_value);
            } else {
                ctx->current_state = ERROR;
                return -1;
            }
            break;
            
        case READING:
            if (input_value >= 0 && ctx->count < 10) {
                ctx->data[ctx->count] = input_value;
                ctx->count++;
                if (ctx->count >= 10) {
                    ctx->current_state = PROCESSING;
                }
                return 0;
            } else {
                ctx->current_state = ERROR;
                return -1;
            }
            break;
            
        case PROCESSING:
            if (ctx->count > 0) {
                ctx->sum = 0;
                for (int i = 0; i < ctx->count; i++) {
                    if (ctx->sum > INT_MAX - ctx->data[i]) {
                        ctx->current_state = ERROR;
                        return -1;
                    }
                    ctx->sum += ctx->data[i];
                }
                ctx->current_state = FINISHED;
                return 0;
            } else {
                ctx->current_state = ERROR;
                return -1;
            }
            break;
            
        case FINISHED:
            return 0;
            
        case ERROR:
            return -1;
    }
    return -1;
}

int process_input(struct Context *ctx, int *inputs, int input_count) {
    if (ctx == NULL || inputs == NULL || input_count <= 0) return -1;
    
    if (input_count == 1) {
        return state_machine(ctx, inputs[0]);
    } else {
        int result = state_machine(ctx, inputs[0]);
        if (result == 0) {
            return process_input(ctx, inputs + 1, input_count - 1);
        } else {
            return result;
        }
    }
}

int main() {
    struct Context ctx = {START};
    int inputs[15];
    int input_count = 0;
    
    printf("Enter up to 15 non-negative integers (negative to stop):\n");
    
    for (int i = 0; i < 15; i++) {
        if (scanf("%d", &inputs[i]) != 1) {
            printf("Invalid input\n");
            return 1;
        }
        if (inputs[i] < 0) {
            break;
        }
        input_count++;
    }
    
    if (input_count == 0) {
        printf("No valid inputs provided\n");
        return 1;
    }
    
    int result = process_input(&ctx, inputs, input_count);
    
    if (result == 0 && ctx.current_state == FINISHED) {
        printf("Successfully processed %d numbers\n", ctx.count);
        printf("Sum: %d\n", ctx.sum);
        printf("Average: %.2f\n", ctx.count > 0 ? (double)ctx.sum / ctx.count : 0.0);
    } else {
        printf("Error occurred during processing\n");
        return 1;
    }
    
    return 0;
}
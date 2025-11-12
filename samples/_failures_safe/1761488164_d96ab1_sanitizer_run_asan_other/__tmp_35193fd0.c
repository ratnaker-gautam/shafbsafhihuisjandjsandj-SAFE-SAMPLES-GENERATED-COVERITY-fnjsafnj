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
    if (input == 2) {
        for (int i = 0; i < ctx->count; i++) {
            ctx->sum += ctx->data[i];
        }
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 3) {
        printf("Sum: %d\n", ctx->sum);
        return START;
    }
    return ERROR;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == 0) {
        printf("Reset from error state\n");
        return START;
    }
    return ERROR;
}

void state_machine(struct Context *ctx, int input) {
    enum State next_state = ctx->current_state;
    
    switch (ctx->current_state) {
        case START:
            next_state = handle_start(ctx, input);
            break;
        case READING:
            next_state = handle_reading(ctx, input);
            break;
        case PROCESSING:
            next_state = handle_processing(ctx, input);
            break;
        case FINISHED:
            next_state = handle_finished(ctx, input);
            break;
        case ERROR:
            next_state = handle_error(ctx, input);
            break;
    }
    
    if (next_state != ctx->current_state) {
        ctx->current_state = next_state;
        state_machine(ctx, 0);
    }
}

int main(void) {
    struct Context ctx = {START, {0}, 0, 0};
    int input;
    
    printf("State Machine Controller\n");
    printf("Commands: 1=Start, 0-100=Data, 2=Process, 3=Display, 0=Reset\n");
    
    while (ctx.current_state != FINISHED || ctx.current_state != ERROR) {
        printf("Current state: %d | Enter command: ", ctx.current_state);
        
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input\n");
            continue;
        }
        
        state_machine(&ctx, input);
        
        if (ctx.current_state == FINISHED) {
            state_machine(&ctx, 3);
        }
        
        if (ctx.current_state == ERROR) {
            printf("Error occurred. Enter 0 to reset.\n");
        }
    }
    
    return 0;
}
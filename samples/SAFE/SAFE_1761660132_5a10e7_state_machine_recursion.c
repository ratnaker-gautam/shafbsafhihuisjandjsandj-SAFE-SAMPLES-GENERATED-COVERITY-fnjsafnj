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
    if (input == 0) {
        ctx->count = 0;
        ctx->sum = 0;
        printf("State machine started. Enter numbers (0 to process, -1 to quit):\n");
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, int input) {
    if (input == 0) {
        if (ctx->count > 0) {
            return PROCESSING;
        } else {
            printf("No data to process.\n");
            return ERROR;
        }
    } else if (input == -1) {
        return FINISHED;
    } else {
        if (ctx->count < 10) {
            ctx->data[ctx->count] = input;
            ctx->count++;
            printf("Added %d. Count: %d\n", input, ctx->count);
            return READING;
        } else {
            printf("Buffer full.\n");
            return ERROR;
        }
    }
}

enum State handle_processing(struct Context *ctx, int input) {
    if (input != 0) return ERROR;
    
    ctx->sum = 0;
    for (int i = 0; i < ctx->count; i++) {
        ctx->sum += ctx->data[i];
    }
    
    printf("Processed %d numbers. Sum: %d\n", ctx->count, ctx->sum);
    ctx->count = 0;
    return FINISHED;
}

enum State handle_finished(struct Context *ctx, int input) {
    if (input == 1) {
        return START;
    }
    return FINISHED;
}

enum State handle_error(struct Context *ctx, int input) {
    if (input == 1) {
        return START;
    }
    return ERROR;
}

void run_state_machine(struct Context *ctx, int input) {
    enum State new_state = transition(ctx, input);
    ctx->current_state = new_state;
}

int main(void) {
    struct Context ctx;
    ctx.current_state = START;
    
    int input;
    char buffer[32];
    
    printf("Enter 0 to begin: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    if (sscanf(buffer, "%d", &input) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    run_state_machine(&ctx, input);
    
    while (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Enter number: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        if (sscanf(buffer, "%d", &input) != 1) {
            printf("Invalid input.\n");
            run_state_machine(&ctx, -999);
        } else {
            run_state_machine(&ctx, input);
        }
    }
    
    if (ctx.current_state == ERROR) {
        printf("Error state reached. Enter 1 to restart: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return 1;
        }
        if (sscanf(buffer, "%d", &input) == 1 && input == 1) {
            run_state_machine(&ctx, input);
        }
    }
    
    printf("Program finished.\n");
    return 0;
}
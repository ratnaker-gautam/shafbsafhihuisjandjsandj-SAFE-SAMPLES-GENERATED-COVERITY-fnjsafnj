//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    int position;
    int depth;
};

enum State handle_start(struct Context *ctx, char input);
enum State handle_reading(struct Context *ctx, char input);
enum State handle_parsing(struct Context *ctx, char input);
enum State handle_finished(struct Context *ctx, char input);
enum State handle_error(struct Context *ctx, char input);

enum State process_input(struct Context *ctx, char input) {
    switch (ctx->current_state) {
        case START: return handle_start(ctx, input);
        case READING: return handle_reading(ctx, input);
        case PARSING: return handle_parsing(ctx, input);
        case FINISHED: return handle_finished(ctx, input);
        case ERROR: return handle_error(ctx, input);
        default: return ERROR;
    }
}

enum State handle_start(struct Context *ctx, char input) {
    if (input == '(') {
        ctx->buffer[0] = input;
        ctx->position = 1;
        ctx->depth = 1;
        return READING;
    }
    return ERROR;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (ctx->position >= 255) return ERROR;
    
    if (input == '(') {
        ctx->depth++;
        ctx->buffer[ctx->position++] = input;
        return READING;
    } else if (input == ')') {
        ctx->depth--;
        ctx->buffer[ctx->position++] = input;
        if (ctx->depth == 0) {
            ctx->buffer[ctx->position] = '\0';
            return PARSING;
        }
        return READING;
    } else if (input >= '0' && input <= '9') {
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    return ERROR;
}

enum State handle_parsing(struct Context *ctx, char input) {
    if (input == '\n' || input == '\0') {
        return FINISHED;
    }
    return ERROR;
}

enum State handle_finished(struct Context *ctx, char input) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, char input) {
    return ERROR;
}

int evaluate_expression(const char *expr, int start, int end) {
    if (start > end) return 0;
    if (expr[start] == '(' && expr[end] == ')') {
        int sum = 0;
        int num = 0;
        int in_num = 0;
        
        for (int i = start + 1; i < end; i++) {
            if (expr[i] >= '0' && expr[i] <= '9') {
                num = num * 10 + (expr[i] - '0');
                in_num = 1;
            } else {
                if (in_num) {
                    sum += num;
                    num = 0;
                    in_num = 0;
                }
            }
        }
        if (in_num) {
            sum += num;
        }
        return sum;
    }
    return 0;
}

void recursive_state_machine(struct Context *ctx, const char *input, int pos) {
    if (input[pos] == '\0') {
        return;
    }
    
    ctx->current_state = process_input(ctx, input[pos]);
    
    if (ctx->current_state == FINISHED) {
        int result = evaluate_expression(ctx->buffer, 0, ctx->position - 1);
        printf("Valid expression: %s = %d\n", ctx->buffer, result);
        return;
    } else if (ctx->current_state == ERROR) {
        printf("Invalid expression at position %d\n", pos);
        return;
    }
    
    recursive_state_machine(ctx, input, pos + 1);
}

int main(void) {
    char input[512];
    
    printf("Enter expressions in format: (numbers separated by spaces)\n");
    printf("Example: (1 2 3)\n");
    printf("Input: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.depth = 0;
    
    recursive_state_machine(&ctx, input, 0);
    
    return 0;
}
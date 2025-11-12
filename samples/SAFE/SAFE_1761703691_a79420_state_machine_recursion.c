//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
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
            return FINISHED;
        }
        return READING;
    } else if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z')) {
        ctx->buffer[ctx->position++] = input;
        return READING;
    }
    return ERROR;
}

enum State handle_parsing(struct Context *ctx, char input) {
    return ERROR;
}

enum State handle_finished(struct Context *ctx, char input) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, char input) {
    return ERROR;
}

void process_string_recursive(struct Context *ctx, const char *str, int index) {
    if (str == NULL || index < 0) return;
    
    if (str[index] == '\0') {
        return;
    }
    
    ctx->current_state = process_input(ctx, str[index]);
    
    if (ctx->current_state == ERROR || ctx->current_state == FINISHED) {
        return;
    }
    
    process_string_recursive(ctx, str, index + 1);
}

int main(void) {
    char input[512];
    
    printf("Enter a nested parentheses expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    struct Context ctx;
    ctx.current_state = START;
    ctx.position = 0;
    ctx.depth = 0;
    
    process_string_recursive(&ctx, input, 0);
    
    switch (ctx.current_state) {
        case FINISHED:
            printf("Valid expression: %s\n", ctx.buffer);
            break;
        case ERROR:
            printf("Invalid expression\n");
            break;
        default:
            printf("Incomplete expression\n");
            break;
    }
    
    return 0;
}
//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_value;
    int recursion_depth;
};

enum State handle_start(struct Context *ctx, char input);
enum State handle_reading(struct Context *ctx, char input);
enum State handle_parsing(struct Context *ctx);
enum State handle_processing(struct Context *ctx);
enum State handle_finished(struct Context *ctx);
enum State handle_error(struct Context *ctx);

enum State process_input(struct Context *ctx, char input) {
    if (ctx == NULL) return ERROR;
    if (ctx->recursion_depth >= 100) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    
    ctx->recursion_depth++;
    
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input);
        case READING:
            return handle_reading(ctx, input);
        case PARSING:
            return handle_parsing(ctx);
        case PROCESSING:
            return handle_processing(ctx);
        case FINISHED:
            return handle_finished(ctx);
        case ERROR:
            return handle_error(ctx);
        default:
            ctx->current_state = ERROR;
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx, char input) {
    if (input == 'B') {
        ctx->current_state = READING;
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
    } else {
        ctx->current_state = ERROR;
    }
    return ctx->current_state;
}

enum State handle_reading(struct Context *ctx, char input) {
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    
    if (input == 'E') {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->current_state = PARSING;
    } else if (input >= '0' && input <= '9') {
        ctx->buffer[ctx->buffer_pos++] = input;
    } else {
        ctx->current_state = ERROR;
    }
    return ctx->current_state;
}

enum State handle_parsing(struct Context *ctx) {
    if (ctx->buffer_pos == 0) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    
    char *endptr;
    long value = strtol(ctx->buffer, &endptr, 10);
    if (endptr == ctx->buffer || *endptr != '\0') {
        ctx->current_state = ERROR;
        return ERROR;
    }
    
    if (value < 0 || value > 10000) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    
    ctx->data_value = (int)value;
    ctx->current_state = PROCESSING;
    return PROCESSING;
}

enum State handle_processing(struct Context *ctx) {
    if (ctx->data_value < 0) {
        ctx->current_state = ERROR;
        return ERROR;
    }
    
    if (ctx->data_value == 0) {
        ctx->current_state = FINISHED;
        return FINISHED;
    }
    
    printf("Processing value: %d\n", ctx->data_value);
    ctx->data_value--;
    
    struct Context new_ctx = *ctx;
    new_ctx.recursion_depth = ctx->recursion_depth;
    enum State result = process_input(&new_ctx, 'X');
    ctx->current_state = result;
    return result;
}

enum State handle_finished(struct Context *ctx) {
    printf("Processing completed successfully.\n");
    return FINISHED;
}

enum State handle_error(struct Context *ctx) {
    printf("Error state reached.\n");
    return ERROR;
}

int main() {
    struct Context ctx;
    ctx.current_state = START;
    ctx.buffer_pos = 0;
    ctx.data_value = 0;
    ctx.recursion_depth = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    printf("Enter sequence starting with 'B', followed by digits, ending with 'E': ");
    
    int input_char;
    while ((input_char = getchar()) != EOF && input_char != '\n') {
        if (ctx.current_state == ERROR || ctx.current_state == FINISHED) {
            break;
        }
        
        enum State result = process_input(&ctx, (char)input_char);
        if (result == ERROR || result == FINISHED) {
            break;
        }
    }
    
    if (ctx.current_state != FINISHED && ctx.current_state != ERROR) {
        printf("Incomplete input sequence.\n");
    }
    
    return ctx.current_state == FINISHED ? 0 : 1;
}
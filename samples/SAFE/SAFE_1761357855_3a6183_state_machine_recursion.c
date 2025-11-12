//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PARSING, PROCESSING, FINISHED, ERROR };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int max_depth;
};

enum State handle_start(struct Context *ctx, const char *input, size_t pos, int depth);
enum State handle_reading(struct Context *ctx, const char *input, size_t pos, int depth);
enum State handle_parsing(struct Context *ctx, const char *input, size_t pos, int depth);
enum State handle_processing(struct Context *ctx, const char *input, size_t pos, int depth);
enum State handle_finished(struct Context *ctx, const char *input, size_t pos, int depth);
enum State handle_error(struct Context *ctx, const char *input, size_t pos, int depth);

enum State process_state(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (depth > ctx->max_depth) {
        ctx->max_depth = depth;
    }
    if (depth > 1000) {
        return ERROR;
    }
    if (ctx == NULL || input == NULL) {
        return ERROR;
    }

    switch (ctx->current_state) {
        case START: return handle_start(ctx, input, pos, depth + 1);
        case READING: return handle_reading(ctx, input, pos, depth + 1);
        case PARSING: return handle_parsing(ctx, input, pos, depth + 1);
        case PROCESSING: return handle_processing(ctx, input, pos, depth + 1);
        case FINISHED: return handle_finished(ctx, input, pos, depth + 1);
        case ERROR: return handle_error(ctx, input, pos, depth + 1);
        default: return ERROR;
    }
}

enum State handle_start(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (input[pos] == '\0') {
        return FINISHED;
    }
    ctx->buffer_pos = 0;
    ctx->current_state = READING;
    return process_state(ctx, input, pos, depth);
}

enum State handle_reading(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (input[pos] == '\0') {
        if (ctx->buffer_pos > 0) {
            ctx->current_state = PARSING;
            return process_state(ctx, input, pos, depth);
        } else {
            return FINISHED;
        }
    }
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        return ERROR;
    }
    ctx->buffer[ctx->buffer_pos++] = input[pos];
    if (input[pos] == ' ') {
        ctx->current_state = PARSING;
        return process_state(ctx, input, pos + 1, depth);
    }
    return process_state(ctx, input, pos + 1, depth);
}

enum State handle_parsing(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->word_count++;
        ctx->buffer_pos = 0;
    }
    ctx->current_state = PROCESSING;
    return process_state(ctx, input, pos, depth);
}

enum State handle_processing(struct Context *ctx, const char *input, size_t pos, int depth) {
    ctx->current_state = READING;
    return process_state(ctx, input, pos, depth);
}

enum State handle_finished(struct Context *ctx, const char *input, size_t pos, int depth) {
    return FINISHED;
}

enum State handle_error(struct Context *ctx, const char *input, size_t pos, int depth) {
    return ERROR;
}

int main(void) {
    char input[512];
    struct Context ctx;
    
    printf("Enter text to count words: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    ctx.current_state = START;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.max_depth = 0;
    
    enum State final_state = process_state(&ctx, input, 0, 0);
    
    if (final_state == FINISHED) {
        printf("Word count: %d\n", ctx.word_count);
        printf("Maximum recursion depth: %d\n", ctx.max_depth);
    } else {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    return 0;
}
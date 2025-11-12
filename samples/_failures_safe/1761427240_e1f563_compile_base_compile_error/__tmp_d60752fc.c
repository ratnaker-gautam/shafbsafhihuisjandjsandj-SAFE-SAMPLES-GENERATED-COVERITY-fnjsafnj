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

enum State transition(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (ctx == NULL || input == NULL || depth > 100) {
        return ERROR;
    }
    
    switch (ctx->current_state) {
        case START:
            return handle_start(ctx, input, pos, depth + 1);
        case READING:
            return handle_reading(ctx, input, pos, depth + 1);
        case PARSING:
            return handle_parsing(ctx, input, pos, depth + 1);
        case PROCESSING:
            return handle_processing(ctx, input, pos, depth + 1);
        case FINISHED:
            return handle_finished(ctx, input, pos, depth + 1);
        case ERROR:
            return handle_error(ctx, input, pos, depth + 1);
        default:
            return ERROR;
    }
}

enum State handle_start(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (ctx == NULL || input == NULL) {
        return ERROR;
    }
    
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->max_depth = depth > ctx->max_depth ? depth : ctx->max_depth;
    
    if (pos >= strlen(input)) {
        ctx->current_state = FINISHED;
    } else {
        ctx->current_state = READING;
    }
    
    return transition(ctx, input, pos, depth);
}

enum State handle_reading(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (ctx == NULL || input == NULL) {
        return ERROR;
    }
    
    size_t input_len = strlen(input);
    if (pos >= input_len) {
        ctx->current_state = PARSING;
        return transition(ctx, input, pos, depth);
    }
    
    if (ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
        ctx->current_state = ERROR;
        return transition(ctx, input, pos, depth);
    }
    
    char c = input[pos];
    if (c == ' ' || c == '\t' || c == '\n') {
        ctx->current_state = PARSING;
    } else {
        ctx->buffer[ctx->buffer_pos++] = c;
        pos++;
    }
    
    return transition(ctx, input, pos, depth);
}

enum State handle_parsing(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (ctx == NULL || input == NULL) {
        return ERROR;
    }
    
    if (ctx->buffer_pos > 0) {
        ctx->buffer[ctx->buffer_pos] = '\0';
        ctx->word_count++;
        ctx->buffer_pos = 0;
    }
    
    size_t input_len = strlen(input);
    if (pos >= input_len) {
        ctx->current_state = PROCESSING;
    } else {
        char c = input[pos];
        if (c != ' ' && c != '\t' && c != '\n') {
            ctx->current_state = READING;
        } else {
            pos++;
        }
    }
    
    return transition(ctx, input, pos, depth);
}

enum State handle_processing(struct Context *ctx, const char *input, size_t pos, int depth) {
    if (ctx == NULL) {
        return ERROR;
    }
    
    ctx->current_state = FINISHED;
    return transition(ctx, input, pos, depth);
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
        return EXIT_FAILURE;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return EXIT_FAILURE;
    }
    
    if (strlen(input) >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    ctx.current_state = START;
    ctx.buffer_pos = 0;
    ctx.word_count = 0;
    ctx.max_depth = 0;
    
    enum State final_state = transition(&ctx, input, 0, 0
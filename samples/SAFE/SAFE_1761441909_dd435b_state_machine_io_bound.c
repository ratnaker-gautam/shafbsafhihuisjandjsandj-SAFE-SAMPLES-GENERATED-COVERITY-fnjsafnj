//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READING,
    PROCESSING,
    OUTPUT,
    ERROR,
    DONE
};

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
    int line_count;
};

void init_context(struct Context *ctx) {
    ctx->current_state = INIT;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    ctx->line_count = 0;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

enum State handle_init(struct Context *ctx, char c) {
    if (c == EOF) {
        return DONE;
    }
    if (c == '\n') {
        ctx->line_count++;
    }
    ctx->char_count++;
    if (!is_whitespace(c)) {
        ctx->buffer[ctx->buffer_pos++] = c;
        return READING;
    }
    return INIT;
}

enum State handle_reading(struct Context *ctx, char c) {
    if (c == EOF) {
        ctx->word_count++;
        return OUTPUT;
    }
    ctx->char_count++;
    if (c == '\n') {
        ctx->line_count++;
    }
    if (is_whitespace(c)) {
        ctx->word_count++;
        return PROCESSING;
    }
    if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->buffer_pos++] = c;
    } else {
        return ERROR;
    }
    return READING;
}

enum State handle_processing(struct Context *ctx, char c) {
    if (c == EOF) {
        return OUTPUT;
    }
    ctx->char_count++;
    if (c == '\n') {
        ctx->line_count++;
    }
    if (!is_whitespace(c)) {
        ctx->buffer_pos = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = c;
        } else {
            return ERROR;
        }
        return READING;
    }
    return PROCESSING;
}

enum State handle_output(struct Context *ctx) {
    printf("Statistics:\n");
    printf("Lines: %d\n", ctx->line_count);
    printf("Words: %d\n", ctx->word_count);
    printf("Characters: %d\n", ctx->char_count);
    return DONE;
}

enum State handle_error(struct Context *ctx) {
    fprintf(stderr, "Error: Buffer overflow occurred\n");
    return DONE;
}

void process_input(struct Context *ctx) {
    int c;
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        if (ctx->current_state == OUTPUT) {
            ctx->current_state = handle_output(ctx);
            continue;
        }
        
        c = getchar();
        if (c == EOF && feof(stdin)) {
            c = EOF;
        } else if (c == EOF) {
            ctx->current_state = ERROR;
            continue;
        }
        
        switch (ctx->current_state) {
            case INIT:
                ctx->current_state = handle_init(ctx, c);
                break;
            case READING:
                ctx->current_state = handle_reading(ctx, c);
                break;
            case PROCESSING:
                ctx->current_state = handle_processing(ctx, c);
                break;
            case OUTPUT:
                break;
            case ERROR:
                ctx->current_state = handle_error(ctx);
                break;
            case DONE:
                break;
        }
    }
    
    if (ctx->current_state == ERROR) {
        handle_error(ctx);
    }
}

int main(void) {
    struct Context ctx;
    init_context(&ctx);
    
    printf("Enter text (Ctrl+D to end input):\n");
    process_input(&ctx);
    
    return 0;
}
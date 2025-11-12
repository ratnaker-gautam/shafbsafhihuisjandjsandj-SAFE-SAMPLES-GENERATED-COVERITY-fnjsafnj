//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum State { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct Context {
    enum State current_state;
    char buffer[256];
    size_t buffer_pos;
    int word_count;
    int char_count;
    int line_count;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = IDLE;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_pos = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    ctx->line_count = 0;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int handle_idle_state(struct Context *ctx, char input) {
    if (input == EOF) {
        ctx->current_state = DONE;
        return 1;
    }
    if (!is_whitespace(input)) {
        ctx->buffer[ctx->buffer_pos++] = input;
        ctx->char_count++;
        ctx->current_state = READING;
    } else if (input == '\n') {
        ctx->line_count++;
    }
    return 0;
}

int handle_reading_state(struct Context *ctx, char input) {
    if (input == EOF) {
        ctx->word_count++;
        ctx->current_state = DONE;
        return 1;
    }
    if (is_whitespace(input)) {
        ctx->word_count++;
        ctx->current_state = IDLE;
        if (input == '\n') {
            ctx->line_count++;
        }
    } else {
        if (ctx->buffer_pos < sizeof(ctx->buffer) - 1) {
            ctx->buffer[ctx->buffer_pos++] = input;
            ctx->char_count++;
        } else {
            ctx->current_state = ERROR;
            return 1;
        }
    }
    return 0;
}

int handle_processing_state(struct Context *ctx) {
    for (size_t i = 0; i < ctx->buffer_pos; i++) {
        if (!isprint(ctx->buffer[i])) {
            ctx->current_state = ERROR;
            return 1;
        }
    }
    ctx->current_state = WRITING;
    return 0;
}

int handle_writing_state(struct Context *ctx) {
    if (ctx->buffer_pos > 0) {
        printf("Word: %.*s\n", (int)ctx->buffer_pos, ctx->buffer);
    }
    ctx->buffer_pos = 0;
    ctx->current_state = IDLE;
    return 0;
}

int handle_error_state(struct Context *ctx) {
    fprintf(stderr, "Error: Invalid input or buffer overflow\n");
    ctx->current_state = DONE;
    return 1;
}

int process_input(struct Context *ctx) {
    int ch;
    int result = 0;
    
    while ((ch = getchar()) != EOF) {
        switch (ctx->current_state) {
            case IDLE:
                result = handle_idle_state(ctx, ch);
                break;
            case READING:
                result = handle_reading_state(ctx, ch);
                break;
            case PROCESSING:
                result = handle_processing_state(ctx);
                break;
            case WRITING:
                result = handle_writing_state(ctx);
                break;
            case ERROR:
                result = handle_error_state(ctx);
                break;
            case DONE:
                return 0;
        }
        if (result != 0) break;
        
        if (ctx->current_state == READING && ctx->buffer_pos >= sizeof(ctx->buffer) - 1) {
            ctx->current_state = PROCESSING;
        }
    }
    
    if (ch == EOF && ctx->current_state == READING) {
        ctx->word_count++;
    }
    
    while (ctx->current_state != DONE && ctx->current_state != ERROR) {
        switch (ctx->current_state) {
            case PROCESSING:
                handle_processing_state(ctx);
                break;
            case WRITING:
                handle_writing_state(ctx);
                break;
            default:
                ctx->current_state = DONE;
                break;
        }
    }
    
    return 0;
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    if (process_input(&ctx) != 0) {
        return EXIT_FAILURE;
    }
    
    if (ctx.current_state == ERROR) {
        return EXIT_FAILURE;
    }
    
    printf("\nStatistics:\n");
    printf("Words: %d\n", ctx.word_count);
    printf("Characters: %d\n", ctx.char_count);
    printf("Lines: %d\n", ctx.line_count);
    
    return EXIT_SUCCESS;
}
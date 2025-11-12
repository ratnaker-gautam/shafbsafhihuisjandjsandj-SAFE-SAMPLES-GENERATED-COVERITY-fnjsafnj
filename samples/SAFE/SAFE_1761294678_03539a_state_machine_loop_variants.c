//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

enum state { IDLE, READING, PROCESSING, WRITING, ERROR, DONE };

struct context {
    enum state current;
    char buffer[256];
    size_t pos;
    int word_count;
    int char_count;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

void process_state_idle(struct context* ctx, const char* input, size_t len) {
    if (input[0] != '\0') {
        ctx->current = READING;
        ctx->pos = 0;
        ctx->word_count = 0;
        ctx->char_count = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
    }
}

void process_state_reading(struct context* ctx, const char* input, size_t len) {
    size_t i = 0;
    while (i < len && ctx->pos < sizeof(ctx->buffer) - 1) {
        ctx->buffer[ctx->pos++] = input[i++];
    }
    ctx->buffer[ctx->pos] = '\0';
    if (ctx->pos >= sizeof(ctx->buffer) - 1 || input[len-1] == '\0') {
        ctx->current = PROCESSING;
    }
}

void process_state_processing(struct context* ctx) {
    int in_word = 0;
    size_t i = 0;
    do {
        if (ctx->buffer[i] == '\0') break;
        if (isalnum(ctx->buffer[i])) {
            ctx->char_count++;
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        i++;
    } while (i < sizeof(ctx->buffer));
    ctx->current = WRITING;
}

void process_state_writing(struct context* ctx) {
    printf("Processed text: %s\n", ctx->buffer);
    printf("Words: %d, Characters: %d\n", ctx->word_count, ctx->char_count);
    ctx->current = DONE;
}

void process_state_error(struct context* ctx) {
    printf("Error: Invalid input detected\n");
    ctx->current = DONE;
}

int main(void) {
    struct context ctx = {IDLE, {0}, 0, 0, 0};
    char input[512];
    
    while (ctx.current != DONE && ctx.current != ERROR) {
        switch (ctx.current) {
            case IDLE:
                printf("Enter text (empty line to exit): ");
                if (fgets(input, sizeof(input), stdin) == NULL) break;
                size_t len = strlen(input);
                if (len > 0 && input[len-1] == '\n') input[--len] = '\0';
                if (!validate_input(input, len)) {
                    ctx.current = ERROR;
                    break;
                }
                process_state_idle(&ctx, input, len);
                break;
                
            case READING:
                process_state_reading(&ctx, input, strlen(input));
                break;
                
            case PROCESSING:
                process_state_processing(&ctx);
                break;
                
            case WRITING:
                process_state_writing(&ctx);
                break;
                
            case ERROR:
                process_state_error(&ctx);
                break;
                
            default:
                ctx.current = ERROR;
                break;
        }
    }
    
    return 0;
}
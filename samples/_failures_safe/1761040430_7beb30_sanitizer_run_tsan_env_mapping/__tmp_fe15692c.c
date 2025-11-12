//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: state_machine
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
    if (input == NULL || len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

void process_state_idle(struct context* ctx, const char* input, size_t len) {
    if (validate_input(input, len)) {
        ctx->current = READING;
        ctx->pos = 0;
        ctx->word_count = 0;
        ctx->char_count = 0;
        memset(ctx->buffer, 0, sizeof(ctx->buffer));
    } else {
        ctx->current = ERROR;
    }
}

void process_state_reading(struct context* ctx, const char* input, size_t len) {
    size_t to_copy = len;
    if (ctx->pos + len >= sizeof(ctx->buffer)) {
        to_copy = sizeof(ctx->buffer) - ctx->pos - 1;
    }
    memcpy(ctx->buffer + ctx->pos, input, to_copy);
    ctx->pos += to_copy;
    ctx->buffer[ctx->pos] = '\0';
    ctx->current = PROCESSING;
}

void process_state_processing(struct context* ctx) {
    int in_word = 0;
    ctx->word_count = 0;
    ctx->char_count = 0;
    
    for (size_t i = 0; i < ctx->pos; i++) {
        if (isalnum(ctx->buffer[i])) {
            ctx->char_count++;
            if (!in_word) {
                ctx->word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    ctx->current = WRITING;
}

void process_state_writing(struct context* ctx) {
    printf("Processed text: %s\n", ctx->buffer);
    printf("Word count: %d\n", ctx->word_count);
    printf("Character count: %d\n", ctx->char_count);
    ctx->current = DONE;
}

void process_state_error(struct context* ctx) {
    printf("Error: Invalid input detected\n");
    ctx->current = DONE;
}

int main(void) {
    struct context ctx = {IDLE, {0}, 0, 0, 0};
    char input[512];
    size_t input_len;
    
    while (ctx.current != DONE) {
        switch (ctx.current) {
            case IDLE:
                printf("Enter text to process (max 255 chars): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    input_len = strlen(input);
                    if (input_len > 0 && input[input_len - 1] == '\n') {
                        input[input_len - 1] = '\0';
                        input_len--;
                    }
                    process_state_idle(&ctx, input, input_len);
                } else {
                    ctx.current = ERROR;
                }
                break;
                
            case READING:
                process_state_reading(&ctx, input, input_len);
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
                
            case DONE:
                break;
        }
    }
    
    return 0;
}
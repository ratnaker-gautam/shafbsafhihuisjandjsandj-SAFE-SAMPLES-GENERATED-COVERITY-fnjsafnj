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
    if (input == NULL || len == 0 || len > 255) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i]) && !isspace(input[i])) return 0;
    }
    return 1;
}

void process_state(struct context* ctx, const char* input) {
    size_t len = strlen(input);
    if (!validate_input(input, len)) {
        ctx->current = ERROR;
        return;
    }

    switch (ctx->current) {
        case IDLE:
            ctx->current = READING;
            ctx->pos = 0;
            ctx->word_count = 0;
            ctx->char_count = 0;
            if (len > 0 && len < sizeof(ctx->buffer)) {
                strncpy(ctx->buffer, input, sizeof(ctx->buffer) - 1);
                ctx->buffer[sizeof(ctx->buffer) - 1] = '\0';
                ctx->pos = len;
            }
            break;

        case READING:
            if (ctx->pos + len < sizeof(ctx->buffer) - 1) {
                strncpy(ctx->buffer + ctx->pos, input, len);
                ctx->pos += len;
                ctx->buffer[ctx->pos] = '\0';
            } else {
                ctx->current = ERROR;
            }
            break;

        case PROCESSING:
            for (size_t i = 0; i < ctx->pos; i++) {
                if (isalnum(ctx->buffer[i])) {
                    ctx->char_count++;
                    if (i == 0 || !isalnum(ctx->buffer[i-1])) {
                        ctx->word_count++;
                    }
                }
            }
            ctx->current = WRITING;
            break;

        case WRITING:
            printf("Characters: %d\n", ctx->char_count);
            printf("Words: %d\n", ctx->word_count);
            ctx->current = DONE;
            break;

        case ERROR:
            printf("Error: Invalid input or buffer overflow\n");
            ctx->current = DONE;
            break;

        case DONE:
            break;
    }
}

int main(void) {
    struct context ctx = {IDLE, {0}, 0, 0, 0};
    char input[256];
    int input_count = 0;

    while (ctx.current != DONE && ctx.current != ERROR) {
        if (ctx.current == IDLE || ctx.current == READING) {
            printf("Enter text (empty line to process): ");
            if (fgets(input, sizeof(input), stdin) == NULL) break;
            
            size_t len = strlen(input);
            if (len > 0 && input[len-1] == '\n') {
                input[len-1] = '\0';
                len--;
            }

            if (len == 0) {
                if (ctx.current == READING) {
                    ctx.current = PROCESSING;
                }
            } else {
                process_state(&ctx, input);
            }
        } else {
            process_state(&ctx, "");
        }

        input_count++;
        if (input_count > 100) {
            ctx.current = ERROR;
        }
    }

    if (ctx.current == ERROR) {
        printf("Error: Too many inputs or invalid state\n");
        return 1;
    }

    return 0;
}
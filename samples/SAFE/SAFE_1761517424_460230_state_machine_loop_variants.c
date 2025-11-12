//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    INIT,
    READING,
    PROCESSING,
    WRITING,
    DONE,
    ERROR
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_items;
};

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

static void process_buffer(struct context *ctx) {
    if (ctx->buffer_len > 0) {
        for (size_t i = 0; i < ctx->buffer_len; i++) {
            if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
            }
        }
        ctx->processed_count++;
    }
}

int main(void) {
    struct context ctx = {INIT, {0}, 0, 0, 5};
    char input[256];
    size_t input_len;
    int running = 1;

    while (running) {
        switch (ctx.current_state) {
            case INIT:
                printf("Enter up to %d text items (empty line to finish):\n", ctx.max_items);
                ctx.current_state = READING;
                break;

            case READING:
                if (fgets(input, sizeof(input), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                input_len = strlen(input);
                if (input_len > 0 && input[input_len - 1] == '\n') {
                    input[--input_len] = '\0';
                }
                if (input_len == 0) {
                    ctx.current_state = DONE;
                    break;
                }
                if (!validate_input(input, input_len)) {
                    ctx.current_state = ERROR;
                    break;
                }
                if (input_len >= sizeof(ctx.buffer)) {
                    ctx.current_state = ERROR;
                    break;
                }
                memcpy(ctx.buffer, input, input_len + 1);
                ctx.buffer_len = input_len;
                ctx.current_state = PROCESSING;
                break;

            case PROCESSING:
                if (ctx.processed_count >= ctx.max_items) {
                    ctx.current_state = DONE;
                    break;
                }
                process_buffer(&ctx);
                ctx.current_state = WRITING;
                break;

            case WRITING:
                printf("Processed: %s\n", ctx.buffer);
                ctx.current_state = READING;
                break;

            case DONE:
                printf("Processed %d items. Exiting.\n", ctx.processed_count);
                running = 0;
                break;

            case ERROR:
                printf("Error occurred. Exiting.\n");
                running = 0;
                break;
        }
    }

    return 0;
}
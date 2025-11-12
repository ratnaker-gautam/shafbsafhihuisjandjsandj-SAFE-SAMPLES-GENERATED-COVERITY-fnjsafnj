//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum state {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    DONE
};

struct context {
    enum state current_state;
    char buffer[256];
    size_t buffer_len;
    int processed_count;
    int max_items;
};

int validate_input(const char* input, size_t len) {
    if (input == NULL || len == 0 || len >= 256) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) {
            return 0;
        }
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_buffer(struct context* ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) {
        return;
    }
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer_len = 0;
    ctx.processed_count = 0;
    ctx.max_items = 5;

    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                printf("Enter text to process (or 'quit' to exit): ");
                fflush(stdout);
                ctx.current_state = READING;
                break;

            case READING: {
                if (fgets(ctx.buffer, sizeof(ctx.buffer), stdin) == NULL) {
                    ctx.current_state = ERROR;
                    break;
                }
                size_t len = strlen(ctx.buffer);
                if (len > 0 && ctx.buffer[len - 1] == '\n') {
                    ctx.buffer[len - 1] = '\0';
                    len--;
                }
                if (len == 0) {
                    ctx.current_state = IDLE;
                    break;
                }
                if (strcmp(ctx.buffer, "quit") == 0) {
                    ctx.current_state = DONE;
                    break;
                }
                if (!validate_input(ctx.buffer, len)) {
                    ctx.current_state = ERROR;
                    break;
                }
                ctx.buffer_len = len;
                ctx.current_state = PROCESSING;
                break;
            }

            case PROCESSING:
                process_buffer(&ctx);
                ctx.current_state = WRITING;
                break;

            case WRITING:
                printf("Processed: %s\n", ctx.buffer);
                ctx.processed_count++;
                if (ctx.processed_count >= ctx.max_items) {
                    ctx.current_state = DONE;
                } else {
                    ctx.current_state = IDLE;
                }
                break;

            case ERROR:
                printf("Error: Invalid input detected\n");
                ctx.current_state = DONE;
                break;

            case DONE:
                break;
        }
    }

    printf("Processing complete. Items processed: %d\n", ctx.processed_count);
    return 0;
}
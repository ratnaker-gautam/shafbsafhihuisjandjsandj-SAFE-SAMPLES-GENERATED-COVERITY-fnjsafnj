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
    struct context ctx = {IDLE, {0}, 0, 0, 5};
    char input[256];
    int result;
    
    while (ctx.current_state != DONE && ctx.current_state != ERROR) {
        switch (ctx.current_state) {
            case IDLE:
                printf("Enter text (or 'quit' to exit): ");
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len - 1] == '\n') {
                        input[len - 1] = '\0';
                        len--;
                    }
                    if (strcmp(input, "quit") == 0) {
                        ctx.current_state = DONE;
                    } else if (validate_input(input, len)) {
                        if (len < sizeof(ctx.buffer)) {
                            memcpy(ctx.buffer, input, len + 1);
                            ctx.buffer_len = len;
                            ctx.current_state = READING;
                        } else {
                            ctx.current_state = ERROR;
                        }
                    } else {
                        ctx.current_state = ERROR;
                    }
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case READING:
                if (ctx.buffer_len > 0) {
                    ctx.current_state = PROCESSING;
                } else {
                    ctx.current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                process_buffer(&ctx);
                ctx.current_state = WRITING;
                break;
                
            case WRITING:
                printf("Processed: %s\n", ctx.buffer);
                if (ctx.processed_count >= ctx.max_items) {
                    ctx.current_state = DONE;
                } else {
                    ctx.current_state = IDLE;
                }
                break;
                
            case ERROR:
                printf("Error occurred during processing\n");
                ctx.current_state = DONE;
                break;
                
            case DONE:
                break;
        }
    }
    
    printf("Finished. Processed %d items.\n", ctx.processed_count);
    return EXIT_SUCCESS;
}
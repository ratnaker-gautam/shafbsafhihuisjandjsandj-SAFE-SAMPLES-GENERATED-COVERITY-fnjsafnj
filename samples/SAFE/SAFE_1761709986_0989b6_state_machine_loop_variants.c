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
    int data_count;
    int max_data;
};

int validate_input(const char* input, size_t len) {
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

void state_idle(struct context* ctx) {
    printf("Enter data (max 255 chars) or 'quit' to exit: ");
    char input[256];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        if (validate_input(input, len)) {
            if (strcmp(input, "quit") == 0) {
                ctx->current_state = DONE;
            } else {
                if (len < sizeof(ctx->buffer)) {
                    memcpy(ctx->buffer, input, len + 1);
                    ctx->buffer_len = len;
                    ctx->current_state = READING;
                } else {
                    ctx->current_state = ERROR;
                }
            }
        } else {
            ctx->current_state = ERROR;
        }
    } else {
        ctx->current_state = ERROR;
    }
}

void state_reading(struct context* ctx) {
    if (ctx->data_count >= ctx->max_data) {
        ctx->current_state = DONE;
        return;
    }
    printf("Read: %s\n", ctx->buffer);
    ctx->data_count++;
    ctx->current_state = PROCESSING;
}

void state_processing(struct context* ctx) {
    for (size_t i = 0; i < ctx->buffer_len; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
        }
    }
    ctx->current_state = WRITING;
}

void state_writing(struct context* ctx) {
    printf("Processed: %s\n", ctx->buffer);
    ctx->current_state = IDLE;
}

void state_error(struct context* ctx) {
    printf("Error occurred. Resetting...\n");
    ctx->buffer[0] = '\0';
    ctx->buffer_len = 0;
    ctx->current_state = IDLE;
}

int main(void) {
    struct context ctx;
    ctx.current_state = IDLE;
    ctx.buffer[0] = '\0';
    ctx.buffer_len = 0;
    ctx.data_count = 0;
    ctx.max_data = 5;

    while (ctx.current_state != DONE) {
        switch (ctx.current_state) {
            case IDLE:
                state_idle(&ctx);
                break;
            case READING:
                state_reading(&ctx);
                break;
            case PROCESSING:
                state_processing(&ctx);
                break;
            case WRITING:
                state_writing(&ctx);
                break;
            case ERROR:
                state_error(&ctx);
                break;
            case DONE:
                break;
        }
    }

    printf("Processed %d data items. Exiting.\n", ctx.data_count);
    return 0;
}